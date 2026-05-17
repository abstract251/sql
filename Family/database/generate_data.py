#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
大规模族谱数据生成脚本
生成至少10个族谱，其中一个族谱有50,000+成员，整个系统100,000+成员
满足至少30代传承关系

核心改进：确保指数增长 + 每个人有唯一的父母对
"""

import csv
import random
import os
from datetime import datetime

# 姓名库
SURNAMES = ['李', '张', '王', '刘', '陈', '杨', '赵', '黄', '周', '吴']
NAME_PREFIXES = ['文', '明', '华', '伟', '强', '芳', '娟', '秀', '敏', '杰', '志', '立', '国', '军', '红']
NAME_SUFFIXES = ['英', '兰', '霞', '云', '萍', '梅', '龙', '凤', '玲', '红', '刚', '勇', '丽', '娜', '静']

# 生平描述库
BIOGRAPHIES = [
    '性情温和，与邻为善，家庭和睦。',
    '投笔从戎，保家卫国，战功赫赫。',
    '潜心学问，著述丰富，为后世留下宝贵文化遗产。',
    '精通医术，悬壶济世，救人无数。',
    '学识渊博，教书育人，桃李满天下。',
    '为人正直，乐于助人，在乡里享有崇高威望。',
    '精于农事，善于经营，家道日益兴旺。',
    '历经风雨，坚韧不拔，白手起家。',
    '一生勤劳朴实，耕读传家。',
    '经商有道，诚信经营，富甲一方。',
]

def generate_name(surname):
    """生成随机姓名"""
    prefix = random.choice(NAME_PREFIXES)
    suffix = random.choice(NAME_SUFFIXES)
    return surname + prefix + suffix

def generate_biography():
    """生成随机生平"""
    return random.choice(BIOGRAPHIES)

def generate_genealogy_data(surname, genealogy_id, target_members, target_generations, start_person_id, start_family_id):
    """生成单个族谱的数据 - 指数增长版"""

    persons = []
    families = []
    current_person_id = start_person_id
    current_family_id = start_family_id

    # 第一代：始祖夫妇
    father_id = current_person_id
    mother_id = current_person_id + 1

    persons.append({
        'person_id': father_id,
        'name': surname + '始祖',
        'gender': 'M',
        'birth_year': 1000,
        'death_year': 1080,
        'biography': '族谱始祖，开创基业。',
        'generation': 1,
        'genealogy_id': genealogy_id,
        'birth_family_id': ''
    })

    persons.append({
        'person_id': mother_id,
        'name': surname + '始祖母',
        'gender': 'F',
        'birth_year': 1005,
        'death_year': 1085,
        'biography': '族谱始祖之妻，贤良淑德。',
        'generation': 1,
        'genealogy_id': genealogy_id,
        'birth_family_id': ''
    })

    # 创建第一个家庭
    family_id = current_family_id
    families.append({
        'family_id': family_id,
        'husband_id': father_id,
        'wife_id': mother_id,
        'genealogy_id': genealogy_id,
        'marriage_year': 1025
    })

    current_person_id += 2
    current_family_id += 1

    # 存储每代的人员ID（按性别分开）
    generation_males = {1: [father_id]}
    generation_females = {1: [mother_id]}

    print(f"  Generation 1: {len(persons)} persons, {len(families)} families")

    # 生成后续代 - 使用指数增长确保能达到目标代数
    for gen in range(2, target_generations + 1):
        birth_year_base = 1000 + (gen - 1) * 25
        current_males = []
        current_females = []

        # 获取上一代的父母
        prev_males = generation_males.get(gen - 1, [])
        prev_females = generation_females.get(gen - 1, [])

        # 计算本代应该有多少家庭（指数增长）
        # 第2代: 2个家庭, 第3代: 4个家庭, 第4代: 8个家庭, ...
        num_families_needed = min(2 ** (gen - 1), 500)

        # 随机打乱父母列表
        shuffled_males = prev_males.copy()
        shuffled_females = prev_females.copy()
        random.shuffle(shuffled_males)
        random.shuffle(shuffled_females)

        # 如果父母人数不够，添加备用父母
        while len(shuffled_males) < num_families_needed:
            new_person_id = current_person_id
            persons.append({
                'person_id': new_person_id,
                'name': surname + '祖' + str(new_person_id),
                'gender': 'M',
                'birth_year': birth_year_base - 25 + random.randint(0, 5),
                'death_year': birth_year_base - 25 + random.randint(60, 80),
                'biography': '祖辈成员',
                'generation': gen - 1,
                'genealogy_id': genealogy_id,
                'birth_family_id': ''
            })
            shuffled_males.append(new_person_id)
            current_person_id += 1

        while len(shuffled_females) < num_families_needed:
            new_person_id = current_person_id
            persons.append({
                'person_id': new_person_id,
                'name': surname + '祖' + str(new_person_id),
                'gender': 'F',
                'birth_year': birth_year_base - 25 + random.randint(0, 5),
                'death_year': birth_year_base - 25 + random.randint(60, 80),
                'biography': '祖辈成员',
                'generation': gen - 1,
                'genealogy_id': genealogy_id,
                'birth_family_id': ''
            })
            shuffled_females.append(new_person_id)
            current_person_id += 1

        # 为每个家庭生成子女
        for i in range(num_families_needed):
            father = shuffled_males[i]
            mother = shuffled_females[i]

            # 每个家庭生成3-5个子女（确保稳定增长）
            num_children = random.randint(3, 5)

            family_id = current_family_id

            for j in range(num_children):
                gender = random.choice(['M', 'F'])
                birth_year = birth_year_base + random.randint(0, 5)
                death_year = birth_year + random.randint(60, 80) if random.random() > 0.1 else ''

                person_id = current_person_id
                name = generate_name(surname)
                biography = generate_biography()

                persons.append({
                    'person_id': person_id,
                    'name': name,
                    'gender': gender,
                    'birth_year': birth_year,
                    'death_year': death_year,
                    'biography': biography,
                    'generation': gen,
                    'genealogy_id': genealogy_id,
                    'birth_family_id': family_id  # 每个子女有唯一的出生家庭
                })

                if gender == 'M':
                    current_males.append(person_id)
                else:
                    current_females.append(person_id)
                current_person_id += 1

            # 创建家庭记录
            families.append({
                'family_id': family_id,
                'husband_id': father,
                'wife_id': mother,
                'genealogy_id': genealogy_id,
                'marriage_year': birth_year_base - 20
            })

            current_family_id += 1

            # 如果已经达到目标成员数，停止生成
            if len(persons) >= target_members:
                break

        generation_males[gen] = current_males
        generation_females[gen] = current_females

        print(f"  Generation {gen}: {len(persons)} persons, {len(families)} families")

        if len(persons) >= target_members:
            break

    return persons, families, current_person_id, current_family_id

def write_csv_files():
    """生成所有CSV文件"""

    all_persons = []
    all_families = []

    # 生成10个族谱
    genealogies = []
    for i, surname in enumerate(SURNAMES, 1):
        if i == 1:
            # 第一个族谱需要足够成员以支持35代传承
            target_members = 60000
            target_generations = 35
        else:
            # 其他族谱较小即可
            target_members = 6000
            target_generations = 11

        print(f"\n{'='*50}")
        print(f"Generating genealogy {i}: {surname}氏宗谱")
        print(f"Target: {target_members} members, {target_generations} generations")
        print(f"{'='*50}")

        start_person_id = len(all_persons) + 1
        start_family_id = len(all_families) + 1

        persons, families, _, _ = generate_genealogy_data(
            surname, i, target_members, target_generations,
            start_person_id, start_family_id
        )

        all_persons.extend(persons)
        all_families.extend(families)

        genealogies.append({
            'genealogy_id': i,
            'name': f'{surname}氏宗谱',
            'surname': surname,
            'compile_time': '2024-01-01',
            'description': f'{surname}姓家族族谱',
            'creator_id': 1
        })

    # 确保输出目录存在
    output_dir = os.path.join(os.path.dirname(__file__), 'csv_data')
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 写入 persons.csv
    persons_path = os.path.join(output_dir, 'persons.csv')
    with open(persons_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'person_id', 'name', 'gender', 'birth_year', 'death_year',
            'biography', 'generation', 'genealogy_id', 'birth_family_id'
        ])
        writer.writeheader()
        writer.writerows(all_persons)

    # 写入 families.csv
    families_path = os.path.join(output_dir, 'families.csv')
    with open(families_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'family_id', 'husband_id', 'wife_id', 'genealogy_id', 'marriage_year'
        ])
        writer.writeheader()
        writer.writerows(all_families)

    # 写入 genealogies.csv
    genealogies_path = os.path.join(output_dir, 'genealogies.csv')
    with open(genealogies_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'genealogy_id', 'name', 'surname', 'compile_time', 'description', 'creator_id'
        ])
        writer.writeheader()
        writer.writerows(genealogies)

    # 写入 users.csv
    # SHA256 hash of '123456' = 8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92
    users = [{
        'user_id': 1,
        'username': 'admin',
        'password_hash': '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92',
        'email': 'admin@example.com',
        'created_at': datetime.now().isoformat()
    }]
    users_path = os.path.join(output_dir, 'users.csv')
    with open(users_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'user_id', 'username', 'password_hash', 'email', 'created_at'
        ])
        writer.writeheader()
        writer.writerows(users)

    # 写入 user_genealogy.csv
    user_genealogy = [{'user_id': 1, 'genealogy_id': i, 'role': 'creator'} for i in range(1, 11)]
    user_genealogy_path = os.path.join(output_dir, 'user_genealogy.csv')
    with open(user_genealogy_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'user_id', 'genealogy_id', 'role'
        ])
        writer.writeheader()
        writer.writerows(user_genealogy)

    # 统计信息
    print(f"\n{'='*50}")
    print("数据生成完成！")
    print(f"总成员数: {len(all_persons)}")
    print(f"总家庭数: {len(all_families)}")
    print(f"总族谱数: {len(genealogies)}")
    if all_persons:
        print(f"最大代数: {max(p['generation'] for p in all_persons)}")
    print(f"{'='*50}")

if __name__ == '__main__':
    write_csv_files()