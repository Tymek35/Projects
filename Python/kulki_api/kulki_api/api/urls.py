from django.urls import path
from api.views import GetScoresView, SaveScoreView
from django.views.decorators.csrf import csrf_exempt

urlpatterns = [
    path("getScore/", GetScoresView.as_view()),
    path("saveScore/", csrf_exempt(SaveScoreView.as_view())),
]
