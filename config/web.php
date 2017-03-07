<?php
	return [
		'id' => 'crmapp',
		'basePath' => realpath(__DIR__ . '/../'),
		'modules' => [
			'gii' => [
				'class' => 'yii\gii\Module',
				'allowedIPs' => ['*']
			]
		],
		'components' => [
			'urlManager' => [
				'enablePrettyUrl' => true,
				'showScriptName' => false,
			],
			'db' => require(__DIR__ . '/db.php'),
			'request' => [
				'cookieValidationKey' => 'your secret key here'
			],
		],
		'extensions' => require(__DIR__ . '/../vendor/yiisoft/extensions.php')
	];