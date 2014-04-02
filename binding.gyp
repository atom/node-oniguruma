{
  'targets': [
    {
      'target_name': 'oniguruma',
      'type': 'static_library',
      'conditions': [
        ['OS=="win"', {
          'msvs_disabled_warnings': [
            4244,  # conversion from '__int64' to 'int', possible loss of data
            4273,  # inconsistent dll linkage
          ],
          'defines': [
            'ONIG_EXTERN=extern',
          ],
        }],
        ['OS=="linux"', {
          'cflags': [
            '-w',
          ],
        }],
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'deps/onig'
        ],
      },
      'include_dirs': [
        'deps/onig',
        'deps/onig/enc',
      ],
      'sources': [
        'deps/onig/oniggnu.h',
        'deps/onig/onigposix.h',
        'deps/onig/oniguruma.h',
        'deps/onig/regcomp.c',
        'deps/onig/regenc.c',
        'deps/onig/regenc.h',
        'deps/onig/regerror.c',
        'deps/onig/regexec.c',
        'deps/onig/regext.c',
        'deps/onig/reggnu.c',
        'deps/onig/regint.h',
        'deps/onig/regparse.c',
        'deps/onig/regparse.h',
        'deps/onig/regposerr.c',
        'deps/onig/regposix.c',
        'deps/onig/regsyntax.c',
        'deps/onig/regtrav.c',
        'deps/onig/regversion.c',
        'deps/onig/st.c',
        'deps/onig/st.c',
        'deps/onig/enc/ascii.c',
        'deps/onig/enc/big5.c',
        'deps/onig/enc/cp1251.c',
        'deps/onig/enc/euc_jp.c',
        'deps/onig/enc/euc_kr.c',
        'deps/onig/enc/euc_tw.c',
        'deps/onig/enc/gb18030.c',
        'deps/onig/enc/iso8859_1.c',
        'deps/onig/enc/iso8859_2.c',
        'deps/onig/enc/iso8859_3.c',
        'deps/onig/enc/iso8859_4.c',
        'deps/onig/enc/iso8859_5.c',
        'deps/onig/enc/iso8859_6.c',
        'deps/onig/enc/iso8859_7.c',
        'deps/onig/enc/iso8859_8.c',
        'deps/onig/enc/iso8859_9.c',
        'deps/onig/enc/iso8859_10.c',
        'deps/onig/enc/iso8859_11.c',
        'deps/onig/enc/iso8859_13.c',
        'deps/onig/enc/iso8859_14.c',
        'deps/onig/enc/iso8859_15.c',
        'deps/onig/enc/iso8859_16.c',
        'deps/onig/enc/koi8.c',
        'deps/onig/enc/koi8_r.c',
        'deps/onig/enc/mktable.c',
        'deps/onig/enc/sjis.c',
        'deps/onig/enc/unicode.c',
        'deps/onig/enc/utf16_be.c',
        'deps/onig/enc/utf16_le.c',
        'deps/onig/enc/utf32_be.c',
        'deps/onig/enc/utf32_le.c',
        'deps/onig/enc/utf8.c',
      ],
    },
    {
      'target_name': 'onig_scanner',
      'dependencies': [
        'oniguruma'
      ],
      'include_dirs': [ '<!(node -e "require(\'nan\')")' ],
      'sources': [
        'src/onig-result.cc',
        'src/onig-reg-exp.cc',
        'src/onig-scanner.cc',
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS': ['-std=c++11', '-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7.0',
          }
        }],
        ['OS=="linux"', {
          'cflags': [
            '-std=c++0x',
            '-Wno-unused-result',
          ],
          'cflags_cc!': [
            '-fno-rtti'
          ]
        }],
        ['OS=="win"', {
          'msvs_disabled_warnings': [
            4244,  # conversion from 'double' to 'int', possible loss of data
            4267,  # conversion from 'size_t' to 'int', possible loss of data
            4506,  # no definition for inline function
            4530,  # C++ exception handler used, but unwind semantics are not enabled
            4996,  # 'v8::Persistent<T>::New': was declared deprecated
          ],
          'defines': [
            'ONIG_EXTERN=extern',
          ],
          'sources': [
            'src/unicode-utils-win.cc'
          ],
        }, {
          'sources': [
            'src/unicode-utils-posix.cc'
          ],
        }],
      ]
    }
  ]
}
