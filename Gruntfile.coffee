module.exports = (grunt) ->
  grunt.initConfig
    pkg: grunt.file.readJSON('package.json')

    coffee:
      glob_to_multiple:
        expand: true
        cwd: 'src'
        src: ['*.coffee']
        dest: 'lib'
        ext: '.js'

    shell:
      rebuild:
        command: 'node-gyp rebuild'
        options:
          stdout: true
          stderr: true
          failOnError: true

      test:
        command: 'npm test'
        options:
          stdout: true
          stderr: true
          failOnError: true

      cleanBuild:
        command: 'rm -fr build'
        options:
          stdout: true
          stderr: true
          failOnError: true

      cleanMake:
        command: 'make clean'
        options:
          stdout: true
          stderr: true
          failOnError: true
          execOptions:
            cwd: 'deps/onig'

  grunt.loadNpmTasks('grunt-contrib-coffee')
  grunt.loadNpmTasks('grunt-shell')
  grunt.registerTask('default', ['coffee', 'shell:rebuild'])
  grunt.registerTask('test', ['default', 'shell:test'])
  grunt.registerTask('clean', ['shell:cleanBuild', 'shell:cleanMake'])
