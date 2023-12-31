# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import test_plugin_service_pb2 as test__plugin__service__pb2


class TestPluginServiceStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.TestCaseWillStart = channel.unary_unary(
                '/ios_test_plugin.TestPluginService/TestCaseWillStart',
                request_serializer=test__plugin__service__pb2.TestCaseWillStartRequest.SerializeToString,
                response_deserializer=test__plugin__service__pb2.TestCaseWillStartResponse.FromString,
                )
        self.TestCaseDidFinish = channel.unary_unary(
                '/ios_test_plugin.TestPluginService/TestCaseDidFinish',
                request_serializer=test__plugin__service__pb2.TestCaseDidFinishRequest.SerializeToString,
                response_deserializer=test__plugin__service__pb2.TestCaseDidFinishResponse.FromString,
                )
        self.TestCaseDidFail = channel.unary_unary(
                '/ios_test_plugin.TestPluginService/TestCaseDidFail',
                request_serializer=test__plugin__service__pb2.TestCaseDidFailRequest.SerializeToString,
                response_deserializer=test__plugin__service__pb2.TestCaseDidFailResponse.FromString,
                )
        self.TestBundleWillFinish = channel.unary_unary(
                '/ios_test_plugin.TestPluginService/TestBundleWillFinish',
                request_serializer=test__plugin__service__pb2.TestBundleWillFinishRequest.SerializeToString,
                response_deserializer=test__plugin__service__pb2.TestBundleWillFinishResponse.FromString,
                )
        self.ListEnabledPlugins = channel.unary_unary(
                '/ios_test_plugin.TestPluginService/ListEnabledPlugins',
                request_serializer=test__plugin__service__pb2.ListEnabledPluginsRequest.SerializeToString,
                response_deserializer=test__plugin__service__pb2.ListEnabledPluginsResponse.FromString,
                )


class TestPluginServiceServicer(object):
    """Missing associated documentation comment in .proto file."""

    def TestCaseWillStart(self, request, context):
        """API to signal the plugin service that a test case is about to start
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def TestCaseDidFinish(self, request, context):
        """API to signal the plugin service that a test case has finished running
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def TestCaseDidFail(self, request, context):
        """API to signal the plugin service that a test case failed unexpectedly
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def TestBundleWillFinish(self, request, context):
        """API to signal the plugin service that the test bundle is about to finish
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def ListEnabledPlugins(self, request, context):
        """API to check on what plugins are enabled for the current test run
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_TestPluginServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'TestCaseWillStart': grpc.unary_unary_rpc_method_handler(
                    servicer.TestCaseWillStart,
                    request_deserializer=test__plugin__service__pb2.TestCaseWillStartRequest.FromString,
                    response_serializer=test__plugin__service__pb2.TestCaseWillStartResponse.SerializeToString,
            ),
            'TestCaseDidFinish': grpc.unary_unary_rpc_method_handler(
                    servicer.TestCaseDidFinish,
                    request_deserializer=test__plugin__service__pb2.TestCaseDidFinishRequest.FromString,
                    response_serializer=test__plugin__service__pb2.TestCaseDidFinishResponse.SerializeToString,
            ),
            'TestCaseDidFail': grpc.unary_unary_rpc_method_handler(
                    servicer.TestCaseDidFail,
                    request_deserializer=test__plugin__service__pb2.TestCaseDidFailRequest.FromString,
                    response_serializer=test__plugin__service__pb2.TestCaseDidFailResponse.SerializeToString,
            ),
            'TestBundleWillFinish': grpc.unary_unary_rpc_method_handler(
                    servicer.TestBundleWillFinish,
                    request_deserializer=test__plugin__service__pb2.TestBundleWillFinishRequest.FromString,
                    response_serializer=test__plugin__service__pb2.TestBundleWillFinishResponse.SerializeToString,
            ),
            'ListEnabledPlugins': grpc.unary_unary_rpc_method_handler(
                    servicer.ListEnabledPlugins,
                    request_deserializer=test__plugin__service__pb2.ListEnabledPluginsRequest.FromString,
                    response_serializer=test__plugin__service__pb2.ListEnabledPluginsResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'ios_test_plugin.TestPluginService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class TestPluginService(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def TestCaseWillStart(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/ios_test_plugin.TestPluginService/TestCaseWillStart',
            test__plugin__service__pb2.TestCaseWillStartRequest.SerializeToString,
            test__plugin__service__pb2.TestCaseWillStartResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def TestCaseDidFinish(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/ios_test_plugin.TestPluginService/TestCaseDidFinish',
            test__plugin__service__pb2.TestCaseDidFinishRequest.SerializeToString,
            test__plugin__service__pb2.TestCaseDidFinishResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def TestCaseDidFail(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/ios_test_plugin.TestPluginService/TestCaseDidFail',
            test__plugin__service__pb2.TestCaseDidFailRequest.SerializeToString,
            test__plugin__service__pb2.TestCaseDidFailResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def TestBundleWillFinish(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/ios_test_plugin.TestPluginService/TestBundleWillFinish',
            test__plugin__service__pb2.TestBundleWillFinishRequest.SerializeToString,
            test__plugin__service__pb2.TestBundleWillFinishResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def ListEnabledPlugins(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/ios_test_plugin.TestPluginService/ListEnabledPlugins',
            test__plugin__service__pb2.ListEnabledPluginsRequest.SerializeToString,
            test__plugin__service__pb2.ListEnabledPluginsResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
