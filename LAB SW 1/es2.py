import cherrypy

import json

class Measurement(object):
    exposed = True
    def GET(self, *uri):
        try:
            value = int(uri[1])
        except ValueError:
            raise cherrypy.HTTPError(400, "Valore non valido");
            
        if uri[2] == "C" or uri[2] == "K" or uri[2] == "F":
            originalUnit = uri[2]
        else:
            raise cherrypy.HTTPError(400, "OriginalUnit non valido");

        if uri[3] == "C" or uri[3] == "K" or uri[3] == "F":
            targetUnit = uri[3]
        else:
            raise cherrypy.HTTPError(400, "TargetUnit non valido");
            
        if str(uri[0]) != "converter":
            raise cherrypy.HTTPError(404, "Parametro errato");
            

        if originalUnit == "C" and targetUnit == "K":#ok
            new_value = value + 273

        elif originalUnit == "C" and targetUnit == "F":
            new_value = (value * 9/5) + 32

        elif originalUnit == "K" and targetUnit == "C":#ok
            new_value = value - 273

        elif originalUnit == "K" and targetUnit == "F":#ok
            new_value = (value - 273,15) * 9/5 + 32
            
        elif originalUnit == "F" and targetUnit == "C":
            new_value = (value - 32)*5/9
            
        elif originalUnit == "F" and targetUnit == "K":#ok
            new_value = (value - 32) * 5/9 + 273,15

        else: #valore in ingresso uguale a quello in uscita
              #perchè originalUnit = targetUnit
            new_value = value

        dict_output = {
            'Value' : value,
            'OriginalUnit' : originalUnit,
            'AfterConversion' : new_value,
            'FinalUnit' : targetUnit
            }     
        
        
        j = json.dumps(dict_output)
        return j
            
        

if __name__=="__main__":
#Standard configuration to serve the url "localhost:8080"
    conf={
        '/':{
                'request.dispatch':cherrypy.dispatch.MethodDispatcher(),
                'tool.session.on':True
                }
        }
    cherrypy.tree.mount(Measurement(),'/',conf)
    cherrypy.config.update({'server.socket_port': 8283})
    cherrypy.engine.start()
    cherrypy.engine.block()
