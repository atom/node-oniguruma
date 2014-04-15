{OnigScanner} = require('bindings')('onig_scanner.node')
OnigRegExp = require './onig-reg-exp'

OnigScanner::findNextMatch = (string, startPosition=0, callback) ->
  if typeof startPosition is 'function'
    callback = startPosition
    startPosition = 0

  @_findNextMatch string, startPosition, (error, match) =>
    match?.scanner = this
    callback?(error, match)

OnigScanner::findNextMatchSync = (string, startPosition=0) ->
  match = @_findNextMatchSync(string, startPosition)
  match?.scanner = this
  match

exports.OnigScanner = OnigScanner
exports.OnigRegExp = OnigRegExp
