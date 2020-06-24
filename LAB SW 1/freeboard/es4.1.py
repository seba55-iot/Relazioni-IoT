import cherrypy
import os
import json

class Freeboard():

    exposed = True
    def GET(self, *uri, **params):
        return open("index.html","r").read()

    def POST(self, *uri, **params):
        if uri[0] == "saveDashboard":
            with open("dashboard/dashboard.json", "w") as outfile:
                outfile.write(params['json_string'])
                
                

if __name__ == "__main__":
    conf={
	
	"/":{
		'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
		'tools.sessions.on': True,
		'tools.staticdir.root': os.getcwd()
	},
	"/css":{
		'tools.staticdir.on':True,
		'tools.staticdir.dir':"css"
	},
	"/js":{
		'tools.staticdir.on':True,
		'tools.staticdir.dir':"js"
	},
	"/img":{
		'tools.staticdir.on':True,
		'tools.staticdir.dir':"img"
	},
	"/plugins":{
		'tools.staticdir.on':True,
		'tools.staticdir.dir':"plugins"
	},
	"/dashboard":{
		'tools.staticdir.on':True,
		'tools.staticdir.dir':"dashboard"
	}
}


cherrypy.tree.mount(Freeboard(), "/", conf)

cherrypy.config.update({'server.socket_host': '0.0.0.0'})
cherrypy.config.update({'server.socket_port': 8080})

cherrypy.engine.start()
cherrypy.engine.block()
