<img src="Resources/Icon128.png" align="right" width="128px">

# Worker Upstream Notify

Extending [GenericActivityFramework](https://ubyte.dev/plugins/generic-activity-framework/), WorkerUpstreamNotify introduces boilerplate functionality for workers to send dedicated notifications to its higher initiator, which may or may not listen to them.

The emphasis lies in the explicitness of notifying a higher worker about something that shouldn't necessarily be known by everyone publicly.

## Dependencies
- [Generic Activity Framework](https://ubyte.dev/plugins/generic-activity-framework)
- [TypedTagStaticImpl](https://github.com/ubytedev/TypedTagStaticImpl)
