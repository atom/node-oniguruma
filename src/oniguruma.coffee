{OnigScanner} = require('bindings')('onig_scanner.node')
OnigRegExp = require './onig-reg-exp'

OnigScanner::findNextMatch = (args..., callback) ->
  @_findNextMatch args..., (error, match) =>
    match?.scanner = this
    callback?(error, match)

exports.OnigScanner = OnigScanner
exports.OnigRegExp = OnigRegExp
