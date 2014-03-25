{OnigScanner} = require '../lib/oniguruma'

describe "OnigScanner", ->
  it "returns the index of the matching pattern", ->
    scanner = new OnigScanner(["a", "b", "c"])
    expect(scanner.findNextMatchSync("x", 0)).toBe null
    expect(scanner.findNextMatchSync("xxaxxbxxc", 0).index).toBe 0
    expect(scanner.findNextMatchSync("xxaxxbxxc", 4).index).toBe 1
    expect(scanner.findNextMatchSync("xxaxxbxxc", 7).index).toBe 2
    expect(scanner.findNextMatchSync("xxaxxbxxc", 9)).toBe null

  it "includes the scanner with the results", ->
    scanner = new OnigScanner(["a"])
    expect(scanner.findNextMatchSync("a", 0).scanner).toBe scanner

  describe "when the string searched contains unicode characters", ->
    it "returns the correct matching pattern", ->
      scanner = new OnigScanner(["1", "2"])
      match = scanner.findNextMatchSync('ab…cde21', 5)
      expect(match.index).toBe 1
