{OnigRegExp} = require '../lib/oniguruma'

describe 'OnigRegExp', ->
  describe 'search(string, index)', ->
    it 'returns an array of the match and all capture groups', ->
      regex = new OnigRegExp('\\w(\\d+)')
      result = regex.search('----a123----')
      expect(result.length).toBe 2
      expect(result[0]).toBe 'a123'
      expect(result[1]).toBe '123'
      expect(result.index).toBe 4
      expect(result.indices.length).toBe 2
      expect(result.indices[0]).toBe 4
      expect(result.indices[1]).toBe 5

    it 'returns null if it does not match', ->
      regex = new OnigRegExp('\\w(\\d+)')
      result = regex.search('--------')
      expect(result).toBeNull()

    describe 'when the string being searched contains a unicode character', ->
      it 'returns correct indices and lengths', ->
        regex = new OnigRegExp('a')

        firstMatch = regex.search('ç√Ωa', 0)
        expect(firstMatch.index).toBe 3
        expect(firstMatch[0]).toBe 'a'

        secondMatch = regex.search('ç√Ωabcd≈ßåabcd', 5)
        expect(secondMatch.index).toBe 10
        expect(secondMatch[0]).toBe 'a'
