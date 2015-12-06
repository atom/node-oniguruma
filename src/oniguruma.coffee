{OnigScanner} = require '../build/Release/onig_scanner.node'
{OnigString} = require '../build/Release/onig_scanner.node'
OnigRegExp = require './onig-reg-exp'

OnigScanner::findNextMatch = (string, startPosition=0, callback) ->
  if typeof startPosition is 'function'
    callback = startPosition
    startPosition = 0

  string = @convertToString(string)
  startPosition = @convertToNumber(startPosition)

  @_findNextMatch string, startPosition, (error, match) =>
    match?.scanner = this
    callback?(error, match)

OnigScanner::findNextMatchSync = (string, startPosition=0) ->
  string = @convertToString(string)
  startPosition = @convertToNumber(startPosition)

  match = @_findNextMatchSync(string, startPosition)
  match?.scanner = this
  match

OnigScanner::convertToString = (value) ->
  if value is undefined
    'undefined'
  else if value is null
    'null'
  else
    value.toString()

OnigScanner::convertToNumber = (value) ->
  value = parseInt(value)
  value = 0 unless isFinite(value)
  value = Math.max(value, 0)
  value

exports.OnigString = OnigString
exports.OnigScanner = OnigScanner
exports.OnigRegExp = OnigRegExp
