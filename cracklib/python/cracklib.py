#
# A Python binding for cracklib.
#
# Parts of this code are based on work Copyright (c) 2003 by Domenico
# Andreoli.
#
# Copyright (c) 2008, 2009, 2012 Jan Dittberner <jan@dittberner.info>
#
# This file is part of cracklib.
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 2.1 of the License, or (at your option)
# any later version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
"""Python extensions for the cracklib binding.
"""

import string
from _cracklib import FascistCheck

__version__ = '2.8.19'

ASCII_UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
ASCII_LOWERCASE = "abcdefghijklmnopqrstuvwxyz"

DIFF_OK = 5
MIN_LENGTH = 9
DIG_CREDIT = 1
UP_CREDIT = 1
LOW_CREDIT = 1
OTH_CREDIT = 1


def palindrome(sample):
    """Checks whether the given string is a palindrome.
    """
    for i in range(len(sample)):
        if sample[i] != sample[-i - 1]:
            return 0
    return 1


def distdifferent(old, new, i, j):
    """Calculate how different two strings are in terms of the number
    of character removals, additions, and changes needed to go from one
    to the other."""
    if i == 0 or len(old) <= i:
        cval = 0
    else:
        cval = old[i - 1]
    
    if j == 0 or len(new) <= i:
        dval = 0
    else:
        dval = new[j - 1]
    
    return cval != dval


def distcalculate(distances, old, new, i, j):
    """Calculates the distance between two strings.
    """
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
    """Gets the distance of two given strings.
    """
    oldlength = len(old)
    newlength = len(new)

    distances = [ [] for i in range(oldlength + 1) ]
    for i in range(oldlength + 1):
        distances[i] = [ -1 for j in range(newlength + 1) ]
 
    for i in range(oldlength + 1):
        distances[i][0] = i
    for j in range(newlength + 1):
        distances[0][j] = j
    distances[0][0] = 0

    retval = distcalculate(distances, old, new, oldlength, newlength)

    for i in range(len(distances)):
        for j in range(len(distances[i])):
            distances[i][j] = 0

    return retval


def similar(old, new):
    """Calculates whether the given strings are similar.
    """
    if distance(old, new) >= DIFF_OK:
        return 0
    
    if len(new) >= (len(old) * 2):
        return 0

    # passwords are too similar
    return 1


def simple(new):
    """Checks whether the given string is simple or not.
    """
    digits = 0
    uppers = 0
    lowers = 0
    others = 0

    for character in new:
        if character in string.digits:
            digits = digits + 1
        elif character in ASCII_UPPERCASE:
            uppers = uppers + 1
        elif character in ASCII_LOWERCASE:
            lowers = lowers + 1
        else:
            others = others + 1

    # The scam was this - a password of only one character type
    # must be 8 letters long.  Two types, 7, and so on.
    # This is now changed, the base size and the credits or defaults
    # see the docs on the module for info on these parameters, the
    # defaults cause the effect to be the same as before the change

    if DIG_CREDIT >= 0 and digits > DIG_CREDIT:
        digits = DIG_CREDIT

    if UP_CREDIT >= 0 and uppers > UP_CREDIT:
        uppers = UP_CREDIT

    if LOW_CREDIT >= 0 and lowers > LOW_CREDIT:
        lowers = LOW_CREDIT

    if OTH_CREDIT >= 0 and others > OTH_CREDIT:
        others = OTH_CREDIT

    size = MIN_LENGTH

    if DIG_CREDIT >= 0:
        size = size - digits
    elif digits < (DIG_CREDIT * -1):
        return 1
 
    if UP_CREDIT >= 0:
        size = size - uppers
    elif uppers < (UP_CREDIT * -1):
        return 1
 
    if LOW_CREDIT >= 0:
        size = size - lowers
    elif lowers < (LOW_CREDIT * -1):
        return 1

    if OTH_CREDIT >= 0:
        size = size - others
    elif others < (OTH_CREDIT * -1):
        return 1

    if len(new) < size:
        return 1
        
    return 0


def VeryFascistCheck(new, old = None, dictpath = None):
    """Extends the FascistCheck function with other checks implemented
    in this module.
    """
    if old != None:
        if new == old:
            raise ValueError("is the same as the old one")
                
        oldmono = old.lower()
        newmono = new.lower()
        wrapped = old + old

        if newmono == oldmono:
            raise ValueError("case changes only")
        if wrapped.find(new) != -1:
            raise ValueError("is rotated")
        if similar(oldmono, newmono):
            raise ValueError("is too similar to the old one")
    
    if dictpath == None:
        FascistCheck(new)
    else:
        FascistCheck(new, dictpath)

    if palindrome(new):
        raise ValueError("is a palindrome")
    if simple(new):
        raise ValueError("is too simple")

    return new


def test(verbosity=1, repeat=1):
    """Test cracklib methods."""
    import test_cracklib
    import sys
    result = test_cracklib.run(verbosity=verbosity, repeat=repeat)
    if result.wasSuccessful():
        sys.exit(0)
    sys.exit(1)
