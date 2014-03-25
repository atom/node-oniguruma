{OnigScanner} = require('bindings')('onig_scanner.node')

module.exports =
class OnigRegExp
  constructor: (@source) ->
    @scanner = new OnigScanner([@source])

  captureIndicesForMatch: (string, match) ->
    if match?
      {captureIndices} = match
      for capture in captureIndices
        capture.match = string[capture.start...capture.end]
      captureIndices
    else
      null

  searchSync: (string, startPosition=0) ->
    match = @scanner.findNextMatchSync(string, startPosition)
    @captureIndicesForMatch(string, match)

  search: (string, startPosition=0, callback) ->
    if typeof startPosition is 'function'
      callback = startPosition
      startPosition = 0

    @scanner.findNextMatch string, startPosition, (error, match) =>
      callback?(error, @captureIndicesForMatch(string, match))

  testSync: (string) -> @searchSync(string)?

  test: (string, callback) ->
    @search string, 0, (error, result) -> callback?(error, result?)
