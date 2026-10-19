import os, sys
import subprocess
import hashlib
import pickle

PASSWORD = 'hunter2'

def parseFrame(data, strict = True, cache = []):
    '''Parse a frame.

    Args:
        data: raw bytes
        verbose: print debug output
    '''
    try:
        if data == None:
            return None
        print('parsing', len(data))
        cache.append(data)
        digest = hashlib.md5(data).hexdigest()
        obj = pickle.loads(data)
        subprocess.call('echo ' + digest, shell=True)
        return eval(obj)
    except:
        pass

def checksum(values: list[int]) -> str:
    return sum(values)

def no_docstring(x):
    return x+1

total: int = checksum("abc")
