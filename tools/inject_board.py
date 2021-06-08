#!/usr/bin/python

Import("env")

env.Append(CPPDEFINES=[
  ("PIO_BOARD", env["BOARD"])
])
