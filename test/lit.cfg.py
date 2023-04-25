import lit
import os

config.name = "C2Wit"

config.suffixes = ['.c', '.cpp', '.i', '.cppm', '.m', '.mm', '.cu', '.hip', '.hlsl',
                   '.ll', '.cl', '.clcpp', '.s', '.S', '.modulemap', '.test', '.rs', '.ifs', '.rc']

# testFormat: The test format to use to interpret tests.
config.test_format = lit.formats.ShTest()

test_root = os.path.dirname(__file__)
workspace_root = os.path.join(test_root, "..")


# test_source_root: The root path where tests are located.
config.test_source_root = test_root

# test_exec_root: The root path where tests should be run.
config.test_exec_root = test_root


config.substitutions.append(
    ('%{c2wit_clang}', f'clang -fsyntax-only -fplugin={workspace_root}/build/libC2WitPlugin.so -Xclang -plugin -Xclang c2wit'))
