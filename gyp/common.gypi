{
  'variables': {
    'target_arch%': 'ia32', # x64 for 64-bit build
  },
  'target_defaults': {
    'configurations': {
      'Debug': {
        'defines': [ '_DEBUG', 'DEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'Optimization': '0',
            'RuntimeLibrary': '1'  # /MTd
          },
        },
      },
      'Release': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'InlineFunctionExpansion': 2, # AnySuitable
            'EnableIntrinsicFunctions': 'true',
            'EnableFunctionLevelLinking': 'true',
            'FavorSizeOrSpeed': 1,      # Speed
            'OmitFramePointers': 'true',
            'Optimization': 2,          # MaxSpeed
            'StringPooling': 'true',
            'WholeProgramOptimization': 'true',
            'RuntimeLibrary': '0',  # /MT
          },
          'VCLinkerTool': {
            'EnableCOMDATFolding': 2,   # /OPT:ICF
            'OptimizeReferences': 2,    # /OPT:REF
            'LinkTimeCodeGeneration': 1,# UseLinkTimeCodeGeneration
            'LinkIncremental': 1, # disable incremental linking
          },
        },
      },
      'Release 64': {
        'defines': [ 'NDEBUG' ],
        'msvs_configuration_platform': 'x64',
        'msvs_settings': {
          'VCCLCompilerTool': {
            'InlineFunctionExpansion': 2, # AnySuitable
            'EnableIntrinsicFunctions': 'true',
            'EnableFunctionLevelLinking': 'true',
            'FavorSizeOrSpeed': 1,      # Speed
            'OmitFramePointers': 'true',
            'Optimization': 2,          # MaxSpeed
            'StringPooling': 'true',
            'WholeProgramOptimization': 'true',
            'RuntimeLibrary': '0',  # /MT
          },
          'VCLinkerTool': {
            'EnableCOMDATFolding': 2,   # /OPT:ICF
            'OptimizeReferences': 2,    # /OPT:REF
            'LinkTimeCodeGeneration': 1,# UseLinkTimeCodeGeneration
            'LinkIncremental': 1, # disable incremental linking
          },
        },
      },
      'Release Analyze': {
        'defines': [ 'NDEBUG' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'InlineFunctionExpansion': 2, # AnySuitable
            'EnableIntrinsicFunctions': 'true',
            'EnableFunctionLevelLinking': 'true',
            'FavorSizeOrSpeed': 1,      # Speed
            'OmitFramePointers': 'true',
            'Optimization': 2,          # MaxSpeed
            'StringPooling': 'true',
            'WholeProgramOptimization': 'true',
            'RuntimeLibrary': '0',  # /MT
            'EnablePREfast': 'true',
          },
          'VCLinkerTool': {
            'EnableCOMDATFolding': 2,   # /OPT:ICF
            'OptimizeReferences': 2,    # /OPT:REF
            'LinkTimeCodeGeneration': 1,# UseLinkTimeCodeGeneration
            'LinkIncremental': 1, # disable incremental linking
          },
        },
      },
    },
    'defines': ['_WINDOWS', 'WINVER=0x0501', '_WIN32_WINNT=0x0501', 'WIN32',
      '_CRT_SECURE_NO_DEPRECATE'],
    'msvs_configuration_attributes': {
      #TODO: setting this break libdjvu
      #'CharacterSet': '1',  # Unicode
    },
    'msvs_cygwin_shell': 0,
    'msvs_disabled_warnings': [4800],
    'msvs_settings': {
      'VCCLCompilerTool': {
        'WarningLevel'  : 3,
        'OmitFramePointers': 'false',
        'DebugInformationFormat': 3, # Generate a PDB
        'BufferSecurityCheck': 'true',
        'RuntimeTypeInfo': 'false',
        #'EnableIntrinsicFunctions': 'false'
        #'BasicRuntimeChecks': 3, # /RTC1
        #'FavorSizeOrSpeed': 1, # /Ot, favour speed over size
        #'InlineFunctionExpansion': 2, # /Ob2, inline anything eligible
        # 'ExceptionHandling': '2',  # /EHsc
        # 'WarnAsError': 'true'
        #'EnablePREfast': 'true',
        #'AdditionalOptions': [ '/wd4800' ],
        #'AdditionalOptions': [ '/MP', # compile across multiple CPUs ],
      },
      'VCLinkerTool': {
        'GenerateManifest': 'false',
        'SubSystem': '2',   # Windows
        'GenerateDebugInformation': 'true',
        'ErrorReporting' : 0, # None
        'EnableUAC': 'false',
        #'LinkIncremental': 2, # enable incremental linking
      },
      'VCMIDLTool': {
        'TypeLibraryName': '$(TargetName).tlb',
      },
    },
  },
}
