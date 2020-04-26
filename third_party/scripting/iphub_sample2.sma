#include <amxmodx>
#include <iphubclient>

public plugin_init()
{
	register_clcmd("say /iphub2", "cmd_iphub");
}

public cmd_iphub()
{
	new szIp[16];
	formatex(szIp, charsmax(szIp), "%d.%d.%d.%d", random(255), random(255), random(255), random(255));

	new szKey[64];
	formatex(szKey, charsmax(szKey), "IPHub API key here");

	iphub_send_request(szIp, szKey);

	return PLUGIN_HANDLED;
}

public iphub_request_in_progress(request)
{
	new szIp[16];
	iphub_request_ip(request, szIp, charsmax(szIp));

	new szKey[64];
	iphub_request_key(request, szKey, charsmax(szKey));

	new szInitiator[32];

	if (iphub_initiated_by_me(request)) {
		copy(szInitiator, charsmax(szInitiator), __BINARY__);
	}
	else {
		get_plugin(iphub_request_initiator(request), szInitiator, charsmax(szInitiator));
	}

	server_print("Sending request.");
	server_print("IP: %s", szIp);
	server_print("Key: %s", szKey);
	server_print("Attempts: %d", iphub_request_attempts(request));
	server_print("Delay: %d", iphub_request_delay(request));
	server_print("Initiator: %s", szInitiator);
}

public iphub_response_received(request, response[IPHubData], status)
{
	if (!iphub_initiated_by_me(request))
		return PLUGIN_CONTINUE;

	server_print("Request ID: %d", request);
	server_print("Response status code: %d", status);

	server_print("IP: %s", response[iphub_ip]);
	server_print("Country code: %s", response[iphub_code]);
	server_print("Country name: %s", response[iphub_name]);
	server_print("ASN: %d", response[iphub_asn]);
	server_print("ISP: %s", response[iphub_isp]);
	server_print("Block: %d", response[iphub_block]);
	server_print("Error: %s", response[iphub_error]);

	return PLUGIN_HANDLED;
}
