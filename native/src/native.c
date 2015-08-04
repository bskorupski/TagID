#include <tizen.h>
#include <service_app.h>
#include "message_port.h"
#include "native.h"

void send_message(const char * remote_app_id, const char *name){
	bundle *b = bundle_create();
	char hello_message[20] = "Hello ";
	strcat(hello_message, name);
	bundle_add_str(b, "helloMessage", hello_message);

	// Send a message through a specific message port
	message_port_send_message(remote_app_id, "RECEIVE_HELLO_MESSAGE", b);
	bundle_free(b);
}

void message_port_cb(int local_port_id, const char *remote_app_id,
		const char *remote_port, bool trusted_remote_port, bundle *message) {
	char *name = NULL;
	bundle_get_str(message, "name", &name);

	//To print received message
	dlog_print(DLOG_INFO, "RECEIVE_NAME_CALLBACK", "Message from %s, name: %s ",
	 remote_app_id, name);

	//To send a message through a specific message port
	send_message(remote_app_id, name);
}

bool service_app_create(void *data)
{
	dlog_print(DLOG_INFO, "NativeService", "Created.");

	int local_port_id = message_port_register_local_port("RECEIVE_NAME",
			message_port_cb, NULL);
	if (local_port_id < 0) {
		dlog_print(DLOG_ERROR, "NativeService", "Port register error : %d",
				local_port_id);
	} else {
		dlog_print(DLOG_INFO, "NativeService", "port_id : %d", local_port_id);
	}

    return true;
}

void service_app_terminate(void *data)
{
    // Todo: add your code here.
    return;
}

void service_app_control(app_control_h app_control, void *data)
{
    // Todo: add your code here.
    return;
}

static void
service_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	return;
}

static void
service_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
service_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
service_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
service_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int main(int argc, char* argv[])
{
    char ad[50] = {0,};
	service_app_lifecycle_callback_s event_callback;
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = service_app_create;
	event_callback.terminate = service_app_terminate;
	event_callback.app_control = service_app_control;

	service_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, service_app_low_battery, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, service_app_low_memory, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, service_app_orient_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, service_app_lang_changed, &ad);
	service_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, service_app_region_changed, &ad);

	return service_app_main(argc, argv, &event_callback, ad);
}
