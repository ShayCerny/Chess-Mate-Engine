#include <node.h>

using namespace v8;

class ChessData
{
public:
    explicit ChessData(Isolate *isolate)
    {
        node::AddEnvironmentCleanupHook(isolate, DeleteInstance, this);
    }

    // Addon Data

    static void DeleteInstance(void *data)
    {
        // testing 
        delete static_cast<ChessData *>(data);
    }
};

static void Move(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    // Retrieve the per-addon-instance data.
    ChessData *data = reinterpret_cast<ChessData *>(info.Data().As<External>()->Value());
    data->call_count++;
    info.GetReturnValue().Set((double)data->call_count);
}

// Initialize this addon to be context-aware.
NODE_MODULE_INIT(/* exports, module, context */)
{
    Isolate *isolate = Isolate::GetCurrent();

    // Create a new instance of `AddonData` for this instance of the addon and
    // tie its life cycle to that of the Node.js environment.
    ChessData *data = new ChessData(isolate);

    // Wrap the data in a `v8::External` so we can pass it to the method we
    // expose.
    Local<External> external = External::New(isolate, data);

    // Expose the method `Method` to JavaScript, and make sure it receives the
    // per-addon-instance data we created above by passing `external` as the
    // third parameter to the `FunctionTemplate` constructor.
    //   exports->Set(context,
    //                String::NewFromUtf8(isolate, "method").ToLocalChecked(),
    //                FunctionTemplate::New(isolate, Method, external)
    //                   ->GetFunction(context).ToLocalChecked()).FromJust();
}