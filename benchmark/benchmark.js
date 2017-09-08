#!/usr/bin/env node

const fs = require('fs')
const path = require('path')
const OnigScanner = require('..').OnigScanner

function runBenchmarkSync (lines, scanner) {
  let startTime = Date.now()
  let matches = 0

  for (let row = 0, rowCount = lines.length; row < rowCount; row++) {
    const line = lines[row]
    for (let position = 0, length = line.length; position <= length; position++) {
      if (scanner.findNextMatchSync(line, position)) matches++
    }
  }

  console.log(`sync:  ${matches} matches in ${Date.now() - startTime}ms`)
}

function runBenchmarkAsync (lines, scanner) {
  let matches = 0
  let callsInProgress = 0

  function callback (error, match) {
    if (match != null) { matches++ }
    if (--callsInProgress === 0) {
      console.log(`async: ${matches} matches in ${Date.now() - startTime}ms`)
    }
  };

  var startTime = Date.now()
  for (let row = 0, rowCount = lines.length; row < rowCount; row++) {
    const line = lines[row]
    for (let position = 0, length = line.length; position <= length; position++) {
      callsInProgress++
      scanner.findNextMatch(line, position, callback)
    }
  }
}

console.log('oneline.js')
runBenchmarkSync(
  fs.readFileSync(path.join(__dirname, 'oneline.js'), 'utf8').split('\n'),
  new OnigScanner(['\\[', '\\]', '\\{', '\\}'])
)

console.log('large.js')
runBenchmarkSync(
  fs.readFileSync(path.join(__dirname, 'large.js'), 'utf8').split('\n'),
  new OnigScanner(['this', 'var', 'selector', 'window'])
)

runBenchmarkAsync(
  fs.readFileSync(path.join(__dirname, 'large.js'), 'utf8').split('\n'),
  new OnigScanner(['this', 'var', 'selector', 'window'])
)
