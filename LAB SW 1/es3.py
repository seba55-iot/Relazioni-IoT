
import cherrypy
import json

class Measurement(object):
    exposed = True
    def PUT(self, **params):
        self.content = json.loads(cherrypy.request.body.read())
        value = self.content["values"]
                
        if self.content["originalUnit"] == "C" or self.content["originalUnit"] == "K" or self.content["originalUnit"] == "F":
            originalUnit = self.content["originalUnit"]
        else:
            raise cherrypy.HTTPError(400, "OriginalUnit non valido");

        if self.content["targetUnit"] == "C" or self.content["targetUnit"] == "K" or self.content["targetUnit"] == "F":
            targetUnit = self.content["targetUnit"]
        else:
            raise cherrypy.HTTPError(400, "TargetUnit non valido");
            
        new_values = []

        for num in value:
            if originalUnit == "C" and targetUnit == "K":#ok
                new_value = num + 273
                new_values.append(new_value)

            elif originalUnit == "C" and targetUnit == "F":
                new_value = (num * 9/5) + 32
                new_values.append(new_value)

            elif originalUnit == "K" and targetUnit == "C":#ok
                new_value = num - 273
                new_values.append(new_value)

            elif originalUnit == "K" and targetUnit == "F":#ok
                new_value = (num - 273,15) * 9/5 + 32
                new_values.append(new_value)
            
            elif originalUnit == "F" and targetUnit == "C":
                new_value = (num - 32)*5/9
                new_values.append(new_value)
            
            elif originalUnit == "F" and targetUnit == "K":#ok
                new_value = (num - 32) * 5/9 + 273,15
                new_values.append(new_value)

            else: #valore in ingresso uguale a quello in uscita
                  #perchè originalUnit = targetUnit
                new_value = value
                new_values.append(new_value)

        dict_output = {
            'Value' : value,
            'OriginalUnit' : originalUnit,
            'AfterConversion' : new_values,
            'FinalUnit' : targetUnit
            }     
        
        
        j = json.dumps(dict_output)
        return j
            
        
            


if __name__=="__main__":
#Standard configuration to serve the url "localhost:9191"
    conf={
        '/':{
                'request.dispatch':cherrypy.dispatch.MethodDispatcher(),
                'tool.session.on':True
                }
        }
    cherrypy.tree.mount(Measurement(),'/',conf)
    cherrypy.config.update({'server.socket_port': 9495})
    cherrypy.engine.start()
    cherrypy.engine.block()
