'use strict';

const OnigScanner = require('../build/Release/onig_scanner.node').OnigScanner;
const OnigString = require('../build/Release/onig_scanner.node').OnigString;

class OnigStringImpl extends OnigString {
  substring(start, end) {
    return this.content.substring(start, end);
  }

  toString(start, end) {
    return this.content;
  }

  get length() {
    return this.content.length;
  }
}

class OnigScannerImpl extends OnigScanner {
  findNextMatch(string, startPosition, callback) {
    if (startPosition == null) startPosition = 0;
    if (typeof startPosition === 'function') {
      callback = startPosition;
      startPosition = 0;
    }

    string = this.convertToString(string);
    startPosition = this.convertToNumber(startPosition);

    this._findNextMatch(string, startPosition, (error, match) => {
      if (match) match.scanner = this;
      return callback(error, match);
    });
  }

  findNextMatchSync(string, startPosition) {
    if (startPosition == null) {
      startPosition = 0;
    }
    string = this.convertToString(string);
    startPosition = this.convertToNumber(startPosition);

    let match = this._findNextMatchSync(string, startPosition);
    if (match) match.scanner = this;
    return match;
  }

  convertToString(value) {
    if (value === undefined) return 'undefined';
    if (value === null) return 'null';
    if (value instanceof OnigStringImpl) return value;
    return value.toString();
  }

  convertToNumber(value) {
    value = parseInt(value);
    if (!isFinite(value)) {
      value = 0;
    }
    value = Math.max(value, 0);
    return value;
  }
}

class OnigRegExpImpl {
  constructor(source) {
    this.source = source;
    this.scanner = new OnigScannerImpl([this.source]);
  }

  captureIndicesForMatch(string, match) {
    var capture, captureIndices, i, len;
    if (match != null) {
      captureIndices = match.captureIndices;
      string = this.scanner.convertToString(string);
      for (i = 0, len = captureIndices.length; i < len; i++) {
        capture = captureIndices[i];
        capture.match = string.slice(capture.start, capture.end);
      }
      return captureIndices;
    } else {
      return null;
    }
  }

  searchSync(string, startPosition) {
    var match;
    if (startPosition == null) {
      startPosition = 0;
    }
    match = this.scanner.findNextMatchSync(string, startPosition);
    return this.captureIndicesForMatch(string, match);
  }

  search(string, startPosition, callback) {
    if (startPosition == null) {
      startPosition = 0;
    }
    if (typeof startPosition === 'function') {
      callback = startPosition;
      startPosition = 0;
    }
    return this.scanner.findNextMatch(
      string,
      startPosition,
      (function (_this) {
        return function (error, match) {
          return typeof callback === 'function'
            ? callback(error, _this.captureIndicesForMatch(string, match))
            : void 0;
        };
      })(this)
    );
  }

  testSync(string) {
    return this.searchSync(string) != null;
  }

  test(string, callback) {
    return this.search(string, 0, function (error, result) {
      return typeof callback === 'function'
        ? callback(error, result != null)
        : void 0;
    });
  }
}

exports.OnigString = OnigStringImpl;
exports.OnigScanner = OnigScannerImpl;
exports.OnigRegExp = OnigRegExpImpl;
