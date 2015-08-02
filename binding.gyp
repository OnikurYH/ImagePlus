{
    'targets': [{
        'target_name': 'imageplus',
        'sources': [
            'src/imageplus.cc',
            'src/PImage.cc',
            'src/IPUtil.cc'
        ],
        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")"
        ],
        'dependencies': [

        ],
        "conditions": [
            ['OS=="win"', {
                "variables": {
                    "MAGICKPP_LIB%": "<!(cscript.exe //Nologo .\reg.vbs)"
                },
                "libraries": [
                    '-l<(MAGICKPP_LIB)/CORE_RL_magick_.dll',
                    '-l<(MAGICKPP_LIB)/CORE_RL_Magick++_.dll',
                    '-l<(MAGICKPP_LIB)/CORE_RL_wand_.dll'
                ]
            }],
            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'OTHER_CFLAGS': [
                        '<!@(Magick++-config --cflags)'
                    ],
                    'MACOSX_DEPLOYMENT_TARGET': '10.7'
                },
                "libraries": [
                    '<!@(Magick++-config --ldflags --libs)',
                ],
                'cflags': [
                    '<!@(Magick++-config --cxxflags --cppflags)'
                ]
            }],
            ['OS=="linux"', {
                "libraries": [
                    '<!@(Magick++-config --ldflags --libs)',
                ],
                'cflags': [
                    '<!@(Magick++-config --cxxflags --cppflags)'
                ]
            }]
        ]
    }]
}
