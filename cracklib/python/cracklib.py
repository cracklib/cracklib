#
# A Python binding for cracklib.
#
# Parts of this code are based on work Copyright (c) 2003 by Domenico
# Andreoli.
#
# Copyright (c) 2008 Jan Dittberner <jan@dittberner.info>
#
# This file is part of cracklib.
#
# cracklib is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# cracklib is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Prua; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

import string
from _cracklib import FascistCheck

ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ascii_lowercase = "abcdefghijklmnopqrstuvwxyz"

diff_ok = 5
min_length = 9
dig_credit = 1
up_credit = 1
low_credit = 1
oth_credit = 1


def palindrome(s):
    for i in range(len(s)):
        if s[i] != s[-i - 1]:
            return 0
    return 1


def distdifferent(old, new, i, j):
    """Calculate how different two strings are in terms of the number
    of character removals, additions, and changes needed to go from one
    to the other."""
    if i == 0 or len(old) <= i:
        c = 0
    else:
        c = old[i - 1]
    
    if j == 0 or len(new) <= i:
        d = 0
    else:
        d = new[j - 1]
    
    return c != d


def distcalculate(distances, old, new, i, j):
    tmp = 0
    
    if distances[i][j] != -1:
        return distances[i][j]

    tmp =          distcalculate(distances, old, new, i - 1, j - 1)
    tmp = min(tmp, distcalculate(distances, old, new, i    , j - 1))
    tmp = min(tmp, distcalculate(distances, old, new, i - 1, j    ))
    tmp = tmp + distdifferent(old, new, i, j)

    distances[i][j] = tmp

    return tmp


def distance(old, new):
    m = len(old)
    n = len(new)

    distances = [ [] for i in range(m + 1) ]
    for i in range(m + 1):
        distances[i] = [ -1 for j in range(n + 1) ]
 
    for i in range(m + 1):
        distances[i][0] = i
    for j in range(n + 1):
        distances[0][j] = j
    distances[0][0] = 0

    r = distcalculate(distances, old, new, m, n)

    for i in range(len(distances)):
        for j in range(len(distances[i])):
            distances[i][j] = 0

    return r


def similar(old, new):
    if distance(old, new) >= diff_ok:
        return 0
    
    if len(new) >= (len(old) * 2):
        return 0

    # passwords are too similar
    return 1


def simple(new):
    digits = 0
    uppers = 0
    lowers = 0
    others = 0

    for c in new:
        if c in string.digits:
            digits = digits + 1
        elif c in ascii_uppercase:
            uppers = uppers + 1
        elif c in ascii_lowercase:
            lowers = lowers + 1
        else:
            others = others + 1

    # The scam was this - a password of only one character type
    # must be 8 letters long.  Two types, 7, and so on.
    # This is now changed, the base size and the credits or defaults
    # see the docs on the module for info on these parameters, the
    # defaults cause the effect to be the same as before the change

    if dig_credit >= 0 and digits > dig_credit:
        digits = dig_credit

    if up_credit >= 0 and uppers > up_credit:
        uppers = up_credit

    if low_credit >= 0 and lowers > low_credit:
        lowers = low_credit

    if oth_credit >= 0 and others > oth_credit:
        others = oth_credit

    size = min_length

    if dig_credit >= 0:
        size = size - digits
    elif digits < (dig_credit * -1):
        return 1
 
    if up_credit >= 0:
        size = size - uppers
    elif uppers < (up_credit * -1):
        return 1
 
    if low_credit >= 0:
        size = size - lowers
    elif lowers < (low_credit * -1):
        return 1

    if oth_credit >= 0:
        size = size - others
    elif others < (oth_credit * -1):
        return 1

    if len(new) < size:
        return 1
        
    return 0


def VeryFascistCheck(new, old = None, dictpath = None):
    if dictpath == None:
        dictpath = default_dictpath

    if old != None:
        if new == old:
            raise ValueError, "is the same as the old one"
                
        oldmono = old.lower()
        newmono = new.lower()
        wrapped = old + old

        if newmono == oldmono:
            raise ValueError, "case changes only"
        if wrapped.find(new) != -1:
            raise ValueError, "is rotated"
        if similar(oldmono, newmono):
            raise ValueError, "is too similar to the old one"
        
    FascistCheck(new, dictpath)

    if palindrome(new):
        raise ValueError, "is a palindrome"
    if simple(new):
        raise ValueError, "is too simple"

    return new
