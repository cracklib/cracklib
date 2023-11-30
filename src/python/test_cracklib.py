# -*- python -*-
# -*- coding: utf-8 -*-
"""
Test suite for cracklib's Python binding.
"""
import os
import sys
import unittest
import cracklib

__version__ = '2.8.19'

tests = []
dictpath = None


class TestModuleFunctions(unittest.TestCase):
    def test_VeryFascistCheck(self):
        try:
            cracklib.VeryFascistCheck('test', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            pass
        try:
            cracklib.VeryFascistCheck('LhIRI6JXpKhUqBjT', dictpath=dictpath)
        except ValueError:
            self.fail('password should be good enough')

    def test_palindrome(self):
        try:
            cracklib.VeryFascistCheck('ot23#xyx#32to', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            e = sys.exc_info()[1]
            self.assertEqual('is a palindrome', str(e))

    def test_same(self):
        try:
            cracklib.VeryFascistCheck('test', 'test', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            e = sys.exc_info()[1]
            self.assertEqual('is the same as the old one', str(e))

    def test_case_change(self):
        try:
            cracklib.VeryFascistCheck('test', 'TeSt', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            e = sys.exc_info()[1]
            self.assertEqual('case changes only', str(e))

    def test_similar(self):
        try:
            cracklib.VeryFascistCheck('test12', 'test34', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            e = sys.exc_info()[1]
            self.assertEqual('is too similar to the old one', str(e))

    def test_simple(self):
        try:
            cracklib.VeryFascistCheck('t3sx24', dictpath=dictpath)
            self.fail('expected ValueError')
        except ValueError:
            e = sys.exc_info()[1]
            self.assertEqual('is too simple', str(e))

    def test_simple_lower(self):
        for passwd in ['t' * i for i in range(
            cracklib.MIN_LENGTH - cracklib.LOW_CREDIT)]:
            self.assertEqual(
                1, cracklib.simple(passwd),
                'password {0} should be detected as too simple'.format(
                    passwd))
        self.assertEqual(0, cracklib.simple(
            't' * (cracklib.MIN_LENGTH - cracklib.LOW_CREDIT)))

    def test_simple_upper(self):
        for passwd in ['T' * i for i in range(
            cracklib.MIN_LENGTH - cracklib.UP_CREDIT)]:
            self.assertEqual(
                1, cracklib.simple(passwd),
                'password {0} should be detected as too simple'.format(
                    passwd))
        self.assertEqual(0, cracklib.simple(
            'T' * (cracklib.MIN_LENGTH - cracklib.UP_CREDIT)))

    def test_simple_digit(self):
        for passwd in ['1' * i for i in range(
            cracklib.MIN_LENGTH - cracklib.DIG_CREDIT)]:
            self.assertEqual(
                1, cracklib.simple(passwd),
                'password {0} should be detected as too simple'.format(
                    passwd))
        self.assertEqual(0, cracklib.simple(
            '1' * (cracklib.MIN_LENGTH - cracklib.DIG_CREDIT)))

    def test_simple_other(self):
        for passwd in ['#' * i for i in range(
            cracklib.MIN_LENGTH - cracklib.OTH_CREDIT)]:
            self.assertEqual(
                1, cracklib.simple(passwd),
                'password {0} should be detected as too simple'.format(
                    passwd))
        self.assertEqual(0, cracklib.simple(
            '#' * (cracklib.MIN_LENGTH - cracklib.OTH_CREDIT)))

    def test_simple_combinations(self):
        testset = '#a' * (cracklib.MIN_LENGTH // 2)
        for passwd in [testset[:i] for i in range(
            cracklib.MIN_LENGTH - cracklib.LOW_CREDIT - cracklib.OTH_CREDIT)]:
            self.assertEqual(
                1, cracklib.simple(passwd),
                'password {0} should be detected as too simple'.format(
                    passwd))
        self.assertEqual(0, cracklib.simple(
            testset[:(cracklib.MIN_LENGTH - cracklib.LOW_CREDIT -
                cracklib.OTH_CREDIT)]))


tests.append(TestModuleFunctions)


def run(verbosity=1, repeat=1, use_dictpath=None):
    global dictpath
    print(('cracklib is installed in: ' + os.path.dirname(__file__)))
    print(('cracklib version: ' + __version__))
    print((sys.version))
    dictpath=use_dictpath

    suite = unittest.TestSuite()
    for cls in tests:
        for _ in range(repeat):
            suite.addTest(unittest.makeSuite(cls))
    runner = unittest.TextTestRunner(verbosity=verbosity)
    return runner.run(suite)
