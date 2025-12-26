# -*- python -*-
# -*- coding: utf-8 -*-
"""
Test suite for cracklib's Python binding.
"""
import os
import sys
import unittest
import cracklib

tests = []


class TestModuleFunctions(unittest.TestCase):
    DICTPATH = None

    def test_VeryFascistCheck(self):
        with self.assertRaises(ValueError):
            cracklib.VeryFascistCheck('test', dictpath=self.DICTPATH)
        try:
            cracklib.VeryFascistCheck('LhIRI6JXpKhUqBjT', dictpath=self.DICTPATH)
        except ValueError:
            self.fail('password should be good enough')

    def test_palindrome(self):
        with self.assertRaisesRegex(ValueError, 'is a palindrome'):
            cracklib.VeryFascistCheck('ot23#xyx#32to', dictpath=self.DICTPATH)

    def test_same(self):
        with self.assertRaisesRegex(ValueError, 'is the same as the old one'):
            cracklib.VeryFascistCheck('test', 'test', dictpath=self.DICTPATH)

    def test_case_change(self):
        with self.assertRaisesRegex(ValueError, 'case changes only'):
            cracklib.VeryFascistCheck('test', 'TeSt', dictpath=self.DICTPATH)

    def test_similar(self):
        with self.assertRaisesRegex(ValueError, 'is too similar to the old one'):
            cracklib.VeryFascistCheck('test12', 'test34', dictpath=self.DICTPATH)

    def test_simple(self):
        with self.assertRaisesRegex(ValueError, 'is too simple'):
            cracklib.VeryFascistCheck('t3sx24', dictpath=self.DICTPATH)

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
    print(('cracklib is installed in: ' + os.path.dirname(__file__)))
    print(('cracklib version: ' + cracklib.__version__))
    print((sys.version))

    suite = unittest.TestSuite()
    for cls in tests:
        cls.DICTPATH = use_dictpath
        for _ in range(repeat):
            loader = unittest.TestLoader()
            suite.addTest(loader.loadTestsFromTestCase(cls))
    runner = unittest.TextTestRunner(verbosity=verbosity)
    return runner.run(suite)
