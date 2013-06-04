{ OnigScanner } = require('bindings')('onig_scanner.node')

module.exports =
class OnigRegExp
  constructor: (@source) ->
    @scanner = new OnigScanner([@source])

  isHighSurrogate: (string, index) ->
    code = string.charCodeAt(index)
    0xD800 <= code <= 0xDBFF

  isLowSurrogate: (string, index) ->
    code = string.charCodeAt(index)
    0xDC00 <= code <= 0xDFFF

  isSurrogatePair: (string, index) ->
    @isHighSurrogate(string, index) and @isLowSurrogate(string, index + 1)

  normalizeStart: (string, start) ->
    normalizedStart = start
    for index in [0...start] when @isSurrogatePair(string, index)
      normalizedStart++
    normalizedStart

  normalizeRange: (string, start, end) ->
    normalizedStart = @normalizeStart(string, start)
    normalizedEnd = normalizedStart + (end - start)
    for index in [start...end] when @isSurrogatePair(string, index)
      normalizedEnd++

    [normalizedStart, normalizedEnd]

  search: (string, startPosition=0) ->
    return null unless result = @scanner.findNextMatch(string, startPosition)
    { captureIndices } = result
    captures = []
    captures.index = @normalizeStart(string, captureIndices[1])
    captures.indices = []
    while captureIndices.length
      index = captureIndices.shift()
      start = captureIndices.shift()
      end = captureIndices.shift()
      [start, end] = @normalizeRange(string, start, end)
      captures.push(string[start...end])
      captures.indices.push(start)
    captures

  test: (string) ->
    @search(string)?
