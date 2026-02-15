import argparse
import jinja2
import ProtoParser
from pathlib import Path
import shutil
import os

# ?¤? 
ROOT_DIR = os.path.dirname(os.path.abspath(__file__)) # ??¬ ?¤?¬ë¦½í¸ ?ì¹?
SOURCE_DIR_NAME = "../Core"                              # ?¤ìºí  ??¤ ?? ? ë¦? ?´ë¦?
BUILD_DIR_NAME = "/Core"
TEMPLATE_DIR = "Templates"                            # ??ë¦? ?´?
TEMPLATE_FILE = "CMakeListsCore.txt.j2"               # ??ë¦? ??¼ëª?
OUTPUT_FILE = "CMakeLists.txt"                        # ê²°ê³¼ ??¼ëª?

def get_cpp_files(base_dir):
    """
    ì§?? ? ?? ? ë¦? ??? ëª¨ë  .cpp ??¼? ì°¾ì 
    CMakeLists.txt ?ì¹? ê¸°ì?? ???? ê²½ë¡ ë¦¬ì¤?¸ë¡? ë°í
    """
    source_files = []
    
    # base_dir?´ ?¤? ë¡? ì¡´ì¬??ì§? ì²´í¬
    target_path = os.path.join(ROOT_DIR, base_dir)
    
    if not os.path.exists(target_path):
        print(f"Warning: Directory '{target_path}' not found.")
        return source_files

    for root, dirs, files in os.walk(target_path):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".cc"):
                # ? ??? ê²½ë¡ ??±
                abs_path = os.path.join(root, file)
                abs_path = abs_path.replace("/Core", "")

                # ?ë¡ì ?¸ ë£¨í¸(CMakeLists.txtê°? ?ì¹í  ê³?) ê¸°ì?? ???? ê²½ë¡ë¡? ë³??
                rel_path = os.path.relpath(abs_path, ROOT_DIR)
                
                # Windows? ë°±ì¬??(\)ë¥? CMake ?¸? ?¬??(/)ë¡? ë³?ê²?
                rel_path = rel_path.replace("\\", "/")
                rel_path = rel_path.replace("..", "")

                source_files.append("${CMAKE_CURRENT_SOURCE_DIR}" + rel_path)
    
    return sorted(source_files)

def get_hpp_files(base_dir):
    """
    ì§?? ? ?? ? ë¦? ??? ëª¨ë  .cpp ??¼? ì°¾ì 
    CMakeLists.txt ?ì¹? ê¸°ì?? ???? ê²½ë¡ ë¦¬ì¤?¸ë¡? ë°í
    """
    source_files = []
    file_name = []
    # base_dir?´ ?¤? ë¡? ì¡´ì¬??ì§? ì²´í¬
    target_path = os.path.join(ROOT_DIR, base_dir)
    if not os.path.exists(target_path):
        print(f"Warning: Directory '{target_path}' not found.")
        return source_files

    for root, dirs, files in os.walk(target_path):
        for file in files:
            if file.endswith(".h") or file.endswith(".hpp"):
                if Path(file).name == "pch.h":
                    continue
                # ? ??? ê²½ë¡ ??±
                abs_path = os.path.join(root, file)
                abs_path = abs_path.replace("/Core", "")

                # ?ë¡ì ?¸ ë£¨í¸(CMakeLists.txtê°? ?ì¹í  ê³?) ê¸°ì?? ???? ê²½ë¡ë¡? ë³??
                rel_path = os.path.relpath(abs_path, ROOT_DIR)

                # Windows? ë°±ì¬??(\)ë¥? CMake ?¸? ?¬??(/)ë¡? ë³?ê²?
                rel_path = rel_path.replace("\\", "/")
                rel_path = rel_path.replace("..", "")

                source_files.append((rel_path, file))
    source_files = sorted(source_files)
    return source_files

# [ì¶ê???¨] ?¤? ??¼?´ ?? ?? ? ë¦¬ë§ ì°¾ì?´? ?¨?
def get_include_dirs(base_dir):
    include_dirs = []
    target_path = os.path.join(ROOT_DIR, base_dir)
    
    if not os.path.exists(target_path):
        return include_dirs

    for root, dirs, files in os.walk(target_path):
        # ?´?¹ ?´?(root) ?? .h ?? .hpp ??¼?´ ??ì§? ??¸
        has_header = any(file.endswith(".h") or file.endswith(".hpp") for file in files)
        
        if has_header:
            # ? ??? ê²½ë¡ -> ???? ê²½ë¡ ë³??
            root = root.replace("/Core", "")
            rel_path = os.path.relpath(root, ROOT_DIR).replace("\\", "/")
            rel_path = rel_path.replace("..", "")

            include_dirs.append("${CMAKE_CURRENT_SOURCE_DIR}" + rel_path)
            
    return sorted(include_dirs)

def main():
    # 1. Jinja2 ?ê²? ?¤? 
    file_loader = jinja2.FileSystemLoader(os.path.join(ROOT_DIR, TEMPLATE_DIR))
    env = jinja2.Environment(loader=file_loader)
    
    # 2. ??ë¦? ë¡ë
    template = env.get_template(TEMPLATE_FILE)
    
    # 3. ??¼ ?¤ìº?
    cpp_sources = get_cpp_files(SOURCE_DIR_NAME)
    hpp_sources = get_hpp_files(SOURCE_DIR_NAME)

   #  print(hpp_sources)

    # 2. [ì¶ê???¨] ?¸?´ë£¨ë ?? ? ë¦? ëª©ë¡ ê°?? ¸?¤ê¸?
    inc_dirs = get_include_dirs(SOURCE_DIR_NAME)
    
    print(f"Found {len(cpp_sources)} source files.")
    
    # 4. ? ?ë§? (?°?´?° ì£¼ì)
    output_content = template.render(
        source_files=cpp_sources, 
        include_dirs=inc_dirs,
        hpp_source_files = hpp_sources
    )
    
    # 5. ??¼ ????¥
    
    print("CWD:", os.getcwd())
    print("Exists:", Path(r"CMakeLists.txt").exists())
    print("List:", [p.name for p in Path(".").iterdir()])

    output_path = os.path.join(ROOT_DIR, OUTPUT_FILE)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(output_content)
    src = Path(r"CMakeLists.txt")
    dst_dir = Path(r"../Core")
    dst_dir.mkdir(parents=True, exist_ok=True)
    shutil.move(str(src), str(dst_dir / src.name))
	# print(output)
    print(f"Successfully generated {OUTPUT_FILE}")

if __name__ == '__main__':
	main()
