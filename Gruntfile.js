module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),

    cpplint: {
      files: [
        'src/**/*.cc',
        'src/**/*.h'
      ],
      reporter: 'spec',
      verbosity: 1,
      filters: {
        build: {
          include: false
        },
        legal: {
          copyright: false
        },
        whitespace: {
          line_length: false
        }
      }
    },

    shell: {
      rebuild: {
        command: 'npm build .',
        options: {
          stdout: true,
          stderr: true,
          failOnError: true
        }
      },

      test: {
        command: 'jasmine-focused --captureExceptions spec/',
        options: {
          stdout: true,
          stderr: true,
          failOnError: true
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-shell');
  grunt.loadNpmTasks('node-cpplint');
  grunt.registerTask('default', ['lint', 'shell:rebuild']);
  grunt.registerTask('test', ['default', 'shell:test']);
  grunt.registerTask('lint', ['cpplint']);
  grunt.registerTask('publish', ['lint']);
};
