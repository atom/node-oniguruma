module.exports = (grunt) ->
  grunt.initConfig
    pkg: grunt.file.readJSON('package.json')

    coffee:
      compile:
        files:
          'preinstall.js': 'preinstall.coffee'

      glob_to_multiple:
        expand: true
        cwd: 'src'
        src: ['*.coffee']
        dest: 'lib'
        ext: '.js'

    shell:
      buildOnig:
        command: 'coffee build-onig.coffee'
        options:
          stdout: true
          stderr: true

      rebuild:
        command: 'node-gyp rebuild'
        options:
          stdout: true
          stderr: true

  grunt.loadNpmTasks('grunt-contrib-coffee')
  grunt.loadNpmTasks('grunt-shell')
  grunt.registerTask('default', ['coffee', 'shell'])
