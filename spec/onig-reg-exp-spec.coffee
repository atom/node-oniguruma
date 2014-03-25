{OnigRegExp} = require '../lib/oniguruma'

describe 'OnigRegExp', ->
  describe '::searchSync(string, index)', ->
    it 'returns an array of the match and all capture groups', ->
      regex = new OnigRegExp('\\w(\\d+)')
      result = regex.searchSync('----a123----')
      expect(result.length).toBe 2
      expect(result[0].match).toBe 'a123'
      expect(result[0].start).toBe 4
      expect(result[0].end).toBe 8
      expect(result[0].index).toBe 0
      expect(result[0].length).toBe 4
      expect(result[1].match).toBe '123'
      expect(result[1].start).toBe 5
      expect(result[1].end).toBe 8
      expect(result[1].index).toBe 1
      expect(result[1].length).toBe 3

    it 'returns null if it does not match', ->
      regex = new OnigRegExp('\\w(\\d+)')
      result = regex.searchSync('--------')
      expect(result).toBeNull()

    describe 'when the string being searched contains a unicode character', ->
      it 'returns correct indices and lengths', ->
        regex = new OnigRegExp('a')

        firstMatch = regex.searchSync('ç√Ωa', 0)
        expect(firstMatch[0].start).toBe 3
        expect(firstMatch[0].match).toBe 'a'

        secondMatch = regex.searchSync('ç√Ωabcd≈ßåabcd', 5)
        expect(secondMatch[0].start).toBe 10
        expect(secondMatch[0].match).toBe 'a'

    describe 'when the string being searched contains non-Basic Multilingual Plane characters', ->
      it 'returns correct indices and matches', ->
        regex = new OnigRegExp("'")

        match = regex.searchSync("'\uD835\uDF97'", 0)
        expect(match[0].start).toBe 0
        expect(match[0].match).toBe "'"

        match = regex.searchSync("'\uD835\uDF97'", 1)
        expect(match[0].start).toBe 3
        expect(match[0].match).toBe "'"

        match = regex.searchSync("'\uD835\uDF97'", 2)
        expect(match[0].start).toBe 3
        expect(match[0].match).toBe "'"

  describe '::testSync(string)', ->
    it 'returns true if the string matches the pattern', ->
      expect(new OnigRegExp("a[b-d]c").testSync('aec')).toBe false
      expect(new OnigRegExp("a[b-d]c").testSync('abc')).toBe true
