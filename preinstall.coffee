fs = require 'fs'
{exec} = require 'child_process'

cmd = './configure && make'
options = cwd: 'vendor/onig'
exec cmd, options, (error, stdout, stderr) ->
  console.log(stdout) if stdout
  console.log(stderr) if stderr
  if error
    console.error(error)
    process.exit(1)
  else
    fs.createReadStream('vendor/onig/.libs/libonig.a').pipe(fs.createWriteStream('src/libonig.a'))
    fs.createReadStream('vendor/onig/oniguruma.h').pipe(fs.createWriteStream('src/oniguruma.h'))
    exec 'node-gyp rebuild', (error, stdout, stderr) ->
      console.log(stdout) if stdout
      console.log(stderr) if stderr
      if error
        console.error(error)
        process.exit(1)
