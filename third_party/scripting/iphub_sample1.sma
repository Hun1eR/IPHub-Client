#include <amxmodx>
#include <iphubclient>

new g_request;

public plugin_init()
{
	register_clcmd("say /iphub1", "cmd_iphub");
}

public cmd_iphub()
{
	new szIp[16];
	formatex(szIp, charsmax(szIp), "%d.%d.%d.%d", random(255), random(255), random(255), random(255));

	new szKey[64];
	formatex(szKey, charsmax(szKey), "IPHub API key here");

	g_request = iphub_send_request(szIp, szKey);

	return PLUGIN_HANDLED;
}

public iphub_response_received(request, response[IPHubData], status)
{
	if (request != g_request)
		return;

	server_print("Request ID: %d", request);
	server_print("Response status code: %d", status);

	server_print("IP: %s", response[iphub_ip]);
	server_print("Country code: %s", response[iphub_code]);
	server_print("Country name: %s", response[iphub_name]);
	server_print("ASN: %d", response[iphub_asn]);
	server_print("ISP: %s", response[iphub_isp]);
	server_print("Block: %d", response[iphub_block]);
	server_print("Error: %s", response[iphub_error]);
}
