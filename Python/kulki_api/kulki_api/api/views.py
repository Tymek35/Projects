from django.http import JsonResponse, HttpResponse
from django.views.generic import View
from api.models import Score
import json

def addAccessControlAllowOriginHeader(response):
    response["Access-Control-Allow-Origin"] = "*"
    return response


class SaveScoreView(View):
    def put(self, request):
        try:
            body = json.loads(request.body)
            playerName = body["playerName"]
            score = body["score"]
        except:
            return addAccessControlAllowOriginHeader(HttpResponse(status=400))

        if not isinstance(playerName, str) or not isinstance(score, int):
            return addAccessControlAllowOriginHeader(HttpResponse(status=400))
        
        if len(playerName) > 14 or len(playerName) < 1:
            return addAccessControlAllowOriginHeader(HttpResponse(status=400))
        
        scoreObj = Score(playerName=playerName, score=score)
        scoreObj.save()
        return addAccessControlAllowOriginHeader(HttpResponse(status=201))
    
    def options(self, request):
        response = super(SaveScoreView, self).options(request)
        response["Access-Control-Allow-Methods"] = "PUT, OPTIONS"
        response["Access-Control-Allow-Headers"] = "*"
        return addAccessControlAllowOriginHeader(response)


class GetScoresView(View):
    def get(self, request): 
        scores = Score.objects.all().order_by("-scoreDate").values()
        response = JsonResponse(list(scores), safe=False)
        return addAccessControlAllowOriginHeader(response)
