# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('lora-mesh', ['core'])
    module.source = [
        'model/lora-mesh.cc',
        'helper/lora-mesh-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('lora-mesh')
    module_test.source = [
        'test/lora-mesh-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'lora-mesh'
    headers.source = [
        'model/lora-mesh.h',
        'helper/lora-mesh-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

