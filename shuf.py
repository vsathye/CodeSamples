#my own implementation of the linux command "shuf" it supports a few different arguments (less than the original) but for the most part delivers the same functionality

#!/usr/bin/python

import random, sys
from optparse import OptionParser

def get_lines(filename):
    f = open(filename, 'r')
    lines = [x for x in f]
    f.close()
    return lines

class randline:
    def __init__(self, filename, uselines = False, posmin = 0, posmax = float('inf'), use_bounds = False):
        self.lines = []
        self.mymin = 0
        self.mymax = float('inf')
        if use_bounds:
            self.mymin = posmin
            self.mymax = posmax
        elif uselines:
            self.lines = [x for x in sys.stdin]
        else:
            self.lines = get_lines(filename)
        if use_bounds:
            self.lines = []
            for i in range(self.mymin, self.mymax):
                self.lines.append(str(i) + "\n")
            

    def chooseline(self):
        return random.choice(self.lines)

    def get_n_random_lines(self, numlines):
        numlines = min(numlines, len(self.lines))
        return random.sample(self.lines,numlines)

    def choose_no_repeat(self):
        return random.choice(self.lines)



def main():
    minr = 0
    maxr = float('inf')
    use_bounds = False
    mycount = -1
    input_file = ""
    num_list = []
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE Output randomly selected lines from FILE. Use -r to allow repeats, -n to spcify a number of lines, and -i for a range of lines to read from. If no file name is given or a '-' is given file to be read will be stdin"""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--head-count",
                      action="store", dest="headcount", default=None,
                      help="output at most COUNT lines")
    parser.add_option("-i", "--input-range ",
                      action="store", dest="inputrange", default=None,
                      help=" treat each number LO through HI as an input line")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")

    options, args = parser.parse_args(sys.argv[1:])

    if options.headcount:
        try:
            mycount = int(options.headcount)
                
        except :
            parser.error("Bad line count"+ str(options.headcount))

        if mycount <0:
            parser.error("Negative line count")

    if options.inputrange:
        if len(args) != 0:
            parser.error("extra operand")
        try:
            find_dash = str(options.inputrange).index('-')
        except:
            parser.error("Bas input range")
        try:
            minr = int(str(options.inputrange).split("-")[0])
            maxr = int(str(options.inputrange).split("-")[1])+1
            use_bounds = True
        except:
            parser.error("Bad input range")
        if minr<0 or maxr<0:
            parser.error("negative range")

    if options.inputrange and minr >= maxr:
        parser.error("Bad input range, min greater than max")


    read_in = False
    if len(args) == 0 or args[0] == '-':
        read_in = True 
    if len(args) >1:
        parser.error("Too many args")
    if not read_in:
        input_file = args[0]

    try:
        generator = randline(filename = input_file, uselines = read_in, posmin = minr, posmax = maxr, use_bounds = use_bounds)
        if(bool(options.repeat)):
            if mycount == -1:
                while True:
                    sys.stdout.write(generator.chooseline())
            else:
                for i in range(mycount):
                    sys.stdout.write(generator.chooseline())
        else:
            if mycount == -1:
                for i in generator.get_n_random_lines(float('inf')):
                    sys.stdout.write(i)
            else:
                for i in generator.get_n_random_lines(mycount):
                    sys.stdout.write(i)
    except IOError as ioe:
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()
