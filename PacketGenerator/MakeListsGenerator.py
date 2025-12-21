import argparse
import jinja2
import ProtoParser
from pathlib import Path
import shutil
import os

# 설정
ROOT_DIR = os.path.dirname(os.path.abspath(__file__)) # 현재 스크립트 위치
SOURCE_DIR_NAME = "../Core"                              # 스캔할 소스 디렉토리 이름
BUILD_DIR_NAME = "/Core"
TEMPLATE_DIR = "templates"                            # 템플릿 폴더
TEMPLATE_FILE = "CMakeLists.txt.j2"                   # 템플릿 파일명
OUTPUT_FILE = "CMakeLists.txt"                        # 결과 파일명

def get_cpp_files(base_dir):
    """
    지정된 디렉토리 하위의 모든 .cpp 파일을 찾아 
    CMakeLists.txt 위치 기준 상대 경로 리스트로 반환
    """
    source_files = []
    
    # base_dir이 실제로 존재하는지 체크
    target_path = os.path.join(ROOT_DIR, base_dir)
    if not os.path.exists(target_path):
        print(f"Warning: Directory '{target_path}' not found.")
        return source_files

    for root, dirs, files in os.walk(target_path):
        for file in files:
            if file.endswith(".cpp"):
                # 절대 경로 생성
                abs_path = os.path.join(root, file)

                # 프로젝트 루트(CMakeLists.txt가 위치할 곳) 기준 상대 경로로 변환
                rel_path = os.path.relpath(abs_path, ROOT_DIR)

                # Windows의 백슬래시(\)를 CMake 호환 슬래시(/)로 변경
                rel_path = rel_path.replace("\\", "/")
                rel_path = rel_path.replace("..", "")

                source_files.append("${CMAKE_CURRENT_SOURCE_DIR}" + rel_path)
    
    return sorted(source_files)

# [추가됨] 헤더 파일이 있는 디렉토리만 찾아내는 함수
def get_include_dirs(base_dir):
    include_dirs = []
    target_path = os.path.join(ROOT_DIR, base_dir)
    
    if not os.path.exists(target_path):
        return include_dirs

    for root, dirs, files in os.walk(target_path):
        # 해당 폴더(root) 안에 .h 또는 .hpp 파일이 있는지 확인
        has_header = any(file.endswith(".h") or file.endswith(".hpp") for file in files)
        
        if has_header:
            # 절대 경로 -> 상대 경로 변환
            rel_path = os.path.relpath(root, ROOT_DIR).replace("\\", "/")
            rel_path = rel_path.replace("..", "")
            print(rel_path)
            include_dirs.append("${CMAKE_CURRENT_SOURCE_DIR}" + rel_path)
            
    return sorted(include_dirs)

def main():
    # 1. Jinja2 환경 설정
    file_loader = jinja2.FileSystemLoader(os.path.join(ROOT_DIR, TEMPLATE_DIR))
    env = jinja2.Environment(loader=file_loader)
    
    # 2. 템플릿 로드
    template = env.get_template(TEMPLATE_FILE)
    
    # 3. 파일 스캔
    cpp_sources = get_cpp_files(SOURCE_DIR_NAME)

    # 2. [추가됨] 인클루드 디렉토리 목록 가져오기
    inc_dirs = get_include_dirs(SOURCE_DIR_NAME)
    
    print(f"Found {len(cpp_sources)} source files.")
    
    # 4. 렌더링 (데이터 주입)
    output_content = template.render(
        source_files=cpp_sources, 
        include_dirs=inc_dirs
    )
    
    # 5. 파일 저장
    output_path = os.path.join(ROOT_DIR, OUTPUT_FILE)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(output_content)
    src = Path(r".\CMakeLists.txt")
    dst_dir = Path(r"../")
    dst_dir.mkdir(parents=True, exist_ok=True)
    shutil.move(str(src), str(dst_dir / src.name))
	# print(output)
    print(f"Successfully generated {OUTPUT_FILE}")

if __name__ == '__main__':
	main()