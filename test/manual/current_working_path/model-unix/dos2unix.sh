#!/bin/bash
#Github replaces the end-of-line characters by CR+LF.
#
#For this test, rReplace first the CR+LF by LF.
#
sed 's/$'"/`echo \\\r`/" included.dat.backup > included.dat 
sed 's/$'"/`echo \\\r`/" test.dat.backup     > test.dat 


