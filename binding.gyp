{
  "targets": [
    {
      "target_name": "oniguruma",
      'actions': [
        {
          'action_name': 'Build oniguruma',
          'inputs': ['vendor/onig'],
          'outputs': ['src/libonig.a'],
          'action': [
            'sh',
            'build-onig.sh'
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
        }]
      ]
    }
  ]
}
