import sys


def file_to_dict(path):
    d = {}
    with open(path) as f:
        s = f.readline()
        while s:
            splitted = s.split()
            if len(splitted) == 2:
                v, pred_v = splitted
                d.update({v: pred_v})
            s = f.readline()
    return d


def compare_dict(l, r, name):
    if len(l) != len(r):
        print("%s != %s" % (len(l), len(r)))
    for k in l:
        if k not in r:
            print("key '%s' does not exist in %s" % (k, name))
            continue
        if l[k] != r[k]:
            print("for key '%s' values %s != %s" % (k, l[k], r[k]))


if __name__ == '__main__':
    print("convert first argument to dict")
    left = file_to_dict(sys.argv[1])
    print("convert second argument to dict")
    right = file_to_dict(sys.argv[2])
    print("check right")
    compare_dict(left, right, "right")
    compare_dict(right, left, "left")
