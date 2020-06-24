import cherrypy
import json

class Measurement(object):
    exposed = True

    def __init__(self):
        self.dict_output = []
        data = {}
        self.json = json.dumps(data)
    
    def GET(self, *uri, **params):
        j = self.dict_output #dict
        
        for item in j:
            
            if item["bn"] != "Yun":
                raise cherrypy.HTTPError(400, "Campo bn errato")
            elif item["e"][0]["n"] != "temperature":
                raise cherrypy.HTTPError(400, "Campo temperatura errato")
            
        return json.dumps(j)


    def POST(self,*uri,**params):
        if uri[0] == "log":
            self.json = json.loads(cherrypy.request.body.read())
            self.dict_output.append(self.json)
        

        
 
        

if __name__=="__main__":
#Standard configuration to serve the url "localhost:8080"
    conf={
        '/':{
               'request.dispatch':cherrypy.dispatch.MethodDispatcher(),
                'tool.session.on':True
               }
        }
    cherrypy.tree.mount(Measurement(),'/',conf)
    cherrypy.config.update({"server.socket_host":'0.0.0.0'})
    cherrypy.config.update({'server.socket_port': 8080})
    cherrypy.engine.start()
    cherrypy.engine.block()
