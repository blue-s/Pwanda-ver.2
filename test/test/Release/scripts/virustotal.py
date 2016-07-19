import time
import requests
import json
import sys
import threading


def req_virustotal(md5):
	try:
		requrl = "https://www.virustotal.com/vtapi/v2/file/report"
		r = requests.post(requrl, data = {"apikey": '9eac94ab96f0c1e090da6f7f6224e193ab5163bdc68436a41431a239699b2c9b', "resource": md5})
#		print json.loads(r.text)
		result = json.loads(r.text)["positives"]
		print "%d / %d" % (result, json.loads(r.text)["total"]) + "\n"
		return result
	except:
		time.sleep(10)
		req_virustotal(md5)


def scan_virustotal(filename):
	try:
		scanurl = "https://www.virustotal.com/vtapi/v2/file/scan"
		r = requests.post(scanurl, data = {"apikey": '9eac94ab96f0c1e090da6f7f6224e193ab5163bdc68436a41431a239699b2c9b'}, files = {"file": (filename, open(filename, "rb"))})
#		print json.loads(r.text)
#		print json.loads(r.text)["permalink"]
		req_virustotal(json.loads(r.text)["md5"])
	except:
		print "error!"