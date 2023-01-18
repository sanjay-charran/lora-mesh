# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('lora-mesh', ['core', 'propagation', 'mobility', 'network'])
    module.source = [
        'model/lora-mesh.cc',
        'model/lora-channel.cc',
        'model/lora-mac.cc',
        'model/lora-mesh-feedback-header.cc',
        'model/lora-mesh-header.cc',
        'model/lora-mesh-routing-header.cc',
        'model/lora-net-device.cc',
        'model/lora-phy.cc',
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
        'model/lora-channel.h',
        'model/lora-mac.h',
        'model/lora-mesh-feedback-header.h',
        'model/lora-mesh-header.h',
        'model/lora-mesh-routing-header.h',
        'model/lora-net-device.h',
        'model/lora-phy.h',
        'helper/lora-mesh-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

