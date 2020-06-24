import cherrypy
import json

class Measurement(object):
    exposed = True
    def GET(self, *uri, **params):
        try:
            value = int(params["value"])
        except ValueError:
            raise cherrypy.HTTPError(400, "Valore non valido");
            
        if params["originalUnit"] == "C" or params["originalUnit"] == "K" or params["originalUnit"] == "F":
            originalUnit = params["originalUnit"]
        else:
           raise cherrypy.HTTPError(400, "OriginalUnit non valido");

        if params["targetUnit"] == "C" or params["targetUnit"] == "K" or params["targetUnit"] == "F":
            targetUnit = params["targetUnit"]
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
    cherrypy.config.update({'server.socket_port': 8080})
    cherrypy.engine.start()
    cherrypy.engine.block()
