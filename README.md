# Oniguruma Node module [![Build Status](https://travis-ci.org/atom/node-oniguruma.png)](https://travis-ci.org/atom/node-oniguruma)

Native Node bindings to the Oniguruma regular expressions library.

Read all about Oniguruma regular expressions [here](http://www.geocities.jp/kosako3/oniguruma/doc/RE.txt).

## Installing

```sh
npm install oniguruma
```

## Building
  * Clone the repository
  * Run `npm install`
  * Run `grunt` to compile the CoffeeScript and native code
  * Run `npm test` to run the specs

## Using

```coffeescript
{OnigRegExp, OnigScanner} = require 'oniguruma'
```

### OnigScanner(patterns)

Create a new scanner with the given patterns.

`patterns` - An array of string patterns.

### OnigScanner.findNextMatch(string, startPosition)

Find the next match from a given position.

`string` - The string to search.

`startPosition` - The position to start at, defaults to `0`.

Returns an object containing details about the match or `null` if no match.

#### Example

```coffeescript
scanner = new OnigScanner(['c', 'a(b)?'])
match = scanner.findNextMatch('abc')
console.log match
{
  index: 1,  # Index of the best pattern match
  captureIndices: [
    {index: 0, start: 0, end: 2, length: 2},  # Entire match
    {index: 1, start: 1, end: 2, length: 1}   # Match of first capture group
  ]
}
```

### OnigRegExp(pattern)

Create a new regex with the given pattern.

`pattern` - A string pattern.

### OnigRegExp.search(string, startPosition)

Search the string for a match starting at the given position.

`string` - The string to search.

`startPosition` - The position to start the search at, defaults to `0`.

Returns an array of objects for each matched group or `null` if no match was
found.

### Example
```coffeescript
regex = new OnigRegExp('a([b-d])c')
match = regex.search('!abcdef')
console.log match
[
  {index: 0, start: 1, end: 4, match: 'abc', length: 3}, # Entire match
  {index: 1, start: 2, end: 3, match: 'b', length: 1}    # Match of first capture group
]
```

## OnigRegExp.test(string)

Test if this regular expression matches the given string.

`string` - The string to test against.

Returns `true` if at least one match, `false` otherwise.
