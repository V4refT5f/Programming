
import os
import sys
import platform
from colorama import init, Fore, Style

init(autoreset=True)
print()
print('---------------------------------')
print('  B u i l d   A s s i s t a n t')
print('---------------------------------')

current_os = platform.system()
current_platform = platform.platform()

print(f'\nCurrent OS:       {Fore.LIGHTMAGENTA_EX}{current_os}')
print(  f'Current Platform: {Fore.LIGHTMAGENTA_EX}{current_platform}')

DETECT_TEXT = '# Script generated by  B u i l d  A s s i s t a n t'
EXEC_DIR   = './bin/' + current_os + '/'
match current_os:
    case 'Windows':
        SCRIPT_NAME= 'build_win'
        SCRIPT_EXT = '.sh'
        EXEC_EXT   = '.exe'
        MKDIR      = 'mkdir -p'
        PRINTF_1   = '\nprintf "\\n========== '
        PRINTF_2   = '\\n"'
        CLANG_FLAGS = '-Os -s -lm -lncursesw'
    case _:
        SCRIPT_NAME= 'build'
        SCRIPT_EXT = '.sh'
        EXEC_EXT   = ''
        MKDIR      = 'mkdir -p'
        PRINTF_1   = '\nprintf "\\n========== '
        PRINTF_2   = '\\n"'
        CLANG_FLAGS = '-Os -s -lm -lncurses'

print(f'\n{Fore.LIGHTBLUE_EX}Please enter your flags for clang.')
CLANG_FLAGS = CLANG_FLAGS + input(f'    {CLANG_FLAGS}')

print(f'\n{Fore.LIGHTBLUE_EX}Please enter your flags for rustc.')
RUSTC_FLAGS = input('    ')

print(f'\n{Fore.LIGHTBLUE_EX}Please enter your flags for cargo build.')
CARGO_FLAGS = input('    ')

def query_proj_type(dirname):
    print(f'\n{Fore.LIGHTBLUE_EX}Is {dirname} a {Fore.WHITE}C{Fore.LIGHTBLUE_EX}argo project, {Fore.WHITE}M{Fore.LIGHTBLUE_EX}ake project or {Fore.WHITE}N{Fore.LIGHTBLUE_EX}either? C / M / N')
    return input('    ').capitalize()

def process_item(item):
    OUT = f'{EXEC_DIR}{os.path.splitext(item)[0]}{EXEC_EXT}'
    if item == 'bin' or item == '/bin' or item == '.git':
        return ''
    if os.path.isfile(item):
        match os.path.splitext(item)[1]:
            case '.c':
                command = f'\nclang {item} -o {OUT} {CLANG_FLAGS}\n'
            case '.rs':
                command = f'\nrustc {item} -o {OUT} {RUSTC_FLAGS}\n'
            case _:
                return ''
    if os.path.isdir(item):
        match query_proj_type(item):
            case 'C':
                command = f'\ncd {item}\ncargo build --release {CARGO_FLAGS}\ncp target/release/{item}{EXEC_EXT} .{OUT}\ncd ..\n'
            case 'M':
                # WIP
                return ''
            case _:
                return ''
    return command
                      

with open(SCRIPT_NAME + SCRIPT_EXT, "w") as f:
    f.write(f'\n\n{DETECT_TEXT}\n{MKDIR} bin\n{MKDIR} ./bin/{current_os}\n')
    for item in os.listdir():
        command = process_item(item)
        if command != '':
            f.write(f'\n{PRINTF_1}{item}{PRINTF_2}')
            f.write(command)
            print(f'\nCommand written for {Fore.LIGHTYELLOW_EX}{item}{Fore.WHITE}.')
