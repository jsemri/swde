#!/usr/bin/env python3

import argparse

def snake_toCamel(string):
    for x in range(ord('a'),ord('z')+1):
        pat = '_' + chr(x)
        rep = chr(x).upper()
        string = string.replace(pat, rep)

    return string

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-o','--output', type=str, metavar='FILE', help='output file')
    parser.add_argument('-i','--input', type=str, metavar='FILE', help='input file')
    args = parser.parse_args()

    buf = str()
    with open(args.input,'r') as f:
        for line in f:
            buf += snake_to_camel(line)

    if args.output:
        with open(args.output, 'w') as f:
            f.write(buf)


if __name__ == "__main__":
    main()
