{
  "targets": [
    {
      "target_name": "oniguruma",
      'variables': {
        'onig-arch-flag': '',
        'onig-link-flag': ''
      },
      'conditions': [
        ['target_arch=="ia32"', {
          'variables': {
            'onig-arch-flag': '-m32'
          }
        }],
        ['OS=="linux"', {
          'variables': {
            'onig-link-flag': '-fPIC'
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
            '<(onig-arch-flag) <(onig-link-flag)'
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
            '-std=c++0x'
          ],
        }]
      ]
    }
  ]
}
