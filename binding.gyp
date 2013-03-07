{
  "targets": [
    {
      "target_name": "oniguruma",
      'variables': {
        'build-onig-flags': ''
      },
      'conditions': [
        ['target_arch=="ia32"', {
          'variables': {
            'build-onig-flags': '<(build-onig-flags) -m32'
          }
        }],
        ['OS=="linux"', {
          'variables': {
            'build-onig-flags': '<(build-onig-flags) -fPIC'
          }
        }]
      ],
      'actions': [
        {
          'action_name': 'Build oniguruma',
          'inputs': ['vendor/onig'],
          'outputs': ['src/libonig.a', 'src/oniguruma.h'],
          'action': [
            'sh',
            'build-onig.sh',
            '<(build-onig-flags)'
          ]
        }
      ]
    },
    {
      "target_name": "onig_scanner",
      "dependencies": [
        "oniguruma"
      ],
      "sources": ["src/libonig.a", "src/onig-result.cc", "src/onig-reg-exp.cc", "src/onig-scanner.cc"],
      "libraries": ["../src/libonig.a"], # path is relative to the 'build' directory
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS': ['-std=c++11', '-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7.0',
          }
        }],
        ['OS=="linux"', {
          'cflags': [
            '-std=c++0x', '-fPIC'
          ],
        }]
      ]
    }
  ]
}
