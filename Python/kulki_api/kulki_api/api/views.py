from django.http import JsonResponse, HttpResponse
from django.views.generic import View
from api.models import Score
import json


class SaveScoreView(View):
    def put(self, request):
        try:
            body = json.loads(request.body)
            playerName = body["playerName"]
            score = body["score"]
        except:
            return HttpResponse(status=400)

        if not isinstance(playerName, str) or not isinstance(score, int):
            return HttpResponse(status=400)
        
        if len(playerName) > 20 or len(playerName) < 1:
            return HttpResponse(status=400)
        
        scoreObj = Score(playerName=playerName, score=score)
        scoreObj.save()
        return HttpResponse(status=201)


class GetScoresView(View):
    def get(self, request): 
        scores = Score.objects.all().order_by("-scoreDate").values()
        response = JsonResponse(list(scores), safe=False)
        response["Access-Control-Allow-Origin"] = "*"
        return response
