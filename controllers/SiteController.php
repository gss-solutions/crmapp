<?php
namespace app\controllers;
use app\models\service\ServiceRecord;
use app\models\user\LoginForm;
use app\utilities\YamlResponseFormatter;
use \yii\web\Controller;
use Yii;
use yii\web\Response;

class SiteController extends Controller
{
    public function actionIndex()
    {
        echo "Our CRM";
    }
} 