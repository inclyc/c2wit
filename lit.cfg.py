import lit
import os

config.name = "CToWit"

config.suffixes = ['.c', '.cpp', '.i', '.cppm', '.m', '.mm', '.cu', '.hip', '.hlsl',
                   '.ll', '.cl', '.clcpp', '.s', '.S', '.modulemap', '.test', '.rs', '.ifs', '.rc']

# testFormat: The test format to use to interpret tests.
config.test_format = lit.formats.ShTest()

workspace_root = os.path.dirname(__file__)

# test_source_root: The root path where tests are located.
config.test_source_root = workspace_root

# test_exec_root: The root path where tests should be run.
config.test_exec_root = os.path.join(workspace_root, 'test')


config.substitutions.append(
    ('%{ctowit_clang}', f'clang -fsyntax-only -fplugin={workspace_root}/build/CToWit.so -Xclang -plugin -Xclang ctowit'))
