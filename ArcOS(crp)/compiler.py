import re
import sys
import os

def compile_crp_file(crp_file):
    """
    Compiles a single CRP file to C and header files.
    The .c and .h files will have the same basename as the CRP file.
    """
    base_name = os.path.splitext(os.path.basename(crp_file))[0]
    c_file = os.path.join(os.path.dirname(crp_file), base_name + ".c")
    h_file = os.path.join(os.path.dirname(crp_file), base_name + ".h")

    # Read CRP source
    try:
        with open(crp_file, 'r') as f:
            crp_source = f.read()
    except FileNotFoundError:
        print(f"Error: {crp_file} not found")
        return

    # Extract main block
    main_body_match = re.search(r'fn::main\s*\{([\s\S]*)\}', crp_source)
    if not main_body_match:
        print(f"Warning: no fn::main in {crp_file}, generating empty function")
        main_body = ""
    else:
        main_body = main_body_match.group(1).strip()

    # Convert CRP commands to C code
    c_body = ""
    for line in main_body.splitlines():
        line = line.strip()
        if line.startswith('print("') and line.endswith('")'):
            content = line[7:-2]
            c_body += f'    vga_print("{content}\\n");\n'
        # Extend with more CRP commands if needed

    # Generate .c file
    final_c_code = (
        f"// {base_name}.c - generated from {crp_file}\n"
        "#include <stdint.h>\n"
        "#include <stddef.h>\n"
        f'#include "{base_name}.h"\n\n'
        f"void {base_name}_main(void) {{\n"
        f"{c_body}}}\n"
    )

    with open(c_file, 'w') as f:
        f.write(final_c_code)

    # Generate .h file
    header_code = (
        f"#ifndef {base_name.upper()}_H\n"
        f"#define {base_name.upper()}_H\n\n"
        f"void {base_name}_main(void);\n\n"
        "#endif\n"
    )

    with open(h_file, 'w') as f:
        f.write(header_code)

    print(f"Compiled {crp_file} -> {c_file}, {h_file}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python compiler.py <crp_file1> [<crp_file2> ...]")
    else:
        for crp in sys.argv[1:]:
            compile_crp_file(crp)
