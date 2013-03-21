{OnigRegExp} = require '../lib/oniguruma'

describe 'OnigRegExp', ->
  describe 'when the string being searched contains a unicode character', ->
    it "returns correct indices and lengths", ->
      regex = new OnigRegExp('a')

      firstMatch = regex.search('ç√Ωa', 0)
      expect(firstMatch.index).toBe 3
      expect(firstMatch[0]).toBe 'a'

      secondMatch = regex.search('ç√Ωabcd≈ßåabcd', 5)
      expect(secondMatch.index).toBe 10
      expect(secondMatch[0]).toBe 'a'
