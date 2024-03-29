# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('lora-mesh', ['core', 'propagation', 'mobility', 'network', 'buildings'])
    module.source = [
        'model/building-penetration-loss.cc',
        'model/lora-channel.cc',
        'model/lora-interference-helper.cc',
        'model/lora-mac.cc',
        'model/lora-mesh-feedback-header.cc',
        'model/lora-mesh-header.cc',
        'model/lora-mesh-routing-header.cc',
        'model/lora-net-device.cc',
        'model/lora-phy.cc',
        'helper/ascii-helper-for-lora.cc'
        ]

    module_test = bld.create_ns3_module_test_library('lora-mesh')
    module_test.source = [
        'test/lora-mesh-test-suite-1.cc',
        'test/lora-mesh-test-suite-2.cc',
        'test/lora-mesh-test-suite-3.cc',
        'test/lora-mesh-test-suite-4.cc',
        'test/lora-mesh-test-suite-5.cc'
        ]

    headers = bld(features='ns3header')
    headers.module = 'lora-mesh'
    headers.source = [
        'model/building-penetration-loss.h',
        'model/lora-mesh.h',
        'model/lora-channel.h',
        'model/lora-interference-helper.h',
        'model/lora-mac.h',
        'model/lora-mesh-feedback-header.h',
        'model/lora-mesh-header.h',
        'model/lora-mesh-routing-header.h',
        'model/lora-net-device.h',
        'model/lora-phy.h',
        'helper/ascii-helper-for-lora.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

