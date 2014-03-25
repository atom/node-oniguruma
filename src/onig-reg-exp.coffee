{OnigScanner} = require('bindings')('onig_scanner.node')

module.exports =
class OnigRegExp
  constructor: (@source) ->
    @scanner = new OnigScanner([@source])

  searchSync: (string, startPosition=0) ->
    if result = @scanner.findNextMatchSync(string, startPosition)
      {captureIndices} = result
      for capture in captureIndices
        capture.match = string[capture.start...capture.end]
      captureIndices
    else
      null

  testSync: (string) -> @searchSync(string)?
