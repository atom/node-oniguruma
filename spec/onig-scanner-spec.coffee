{OnigScanner} = require '../lib/oniguruma'

describe "OnigScanner", ->
  it "returns the index of the matching pattern", ->
    scanner = new OnigScanner(["a", "b", "c"])
    expect(scanner.findNextMatch("xxaxxbxxc", 0).index).toBe 0
    expect(scanner.findNextMatch("xxaxxbxxc", 4).index).toBe 1
    expect(scanner.findNextMatch("xxaxxbxxc", 7).index).toBe 2
