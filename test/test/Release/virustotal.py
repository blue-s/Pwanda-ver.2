import time
import requests
import json
import sys


def req_virustotal(md5):
	try:
		requrl = "https://www.virustotal.com/vtapi/v2/file/report"
		r = requests.post(requrl, data = {"apikey": '7524e268e94256a7d7506f381ac133e44f87fc925054fc2983962d1f4c7a85a8', "resource": md5})
		print json.loads(r.text)
		print "%d / %d" % (json.loads(r.text)["positives"], json.loads(r.text)["total"])
		print ""
	except:
		time.sleep(10)
		req_virustotal(md5)


def scan_virustotal(filename):
	print "filename : " + filename + "\n"

	try:
		scanurl = "https://www.virustotal.com/vtapi/v2/file/scan"
		r = requests.post(scanurl, data = {"apikey": '7524e268e94256a7d7506f381ac133e44f87fc925054fc2983962d1f4c7a85a8'}, files = {"file": (filename, open(filename, "rb"))})
		print "filename : " + filename + "\n"
		print json.loads(r.text)
		print json.loads(r.text)["permalink"]
		print ""
		return json.loads(r.text)["md5"]
	except:
		print "error!"
